// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�𸮾� ������Ʈ ���� �� ���λ��� ����.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, -90.0f, 0.0f));

	//���� �ҷ�����.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_KWANG(TEXT("SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood'"));
	if (SM_KWANG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_KWANG.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //Ŭ�������� ��������.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//GTA�� ControlMode ����.
	ExpectedSpringArmLength = 450.0f;
	SpringArm->TargetArmLength = ExpectedSpringArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;//ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ� ȸ�������ִ� ���.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;//��Ʈ�� ȸ���� ����Ű�� �������� ĳ���Ͱ� �ε巴�� ȸ��.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 800.0f; //���� ���� ����
	bIsAttacking = false;//�������� �ƴ����� �ʱ�ȭ.
	MaxCombo = 4; //�ִ� �޺� ����.
	AttackEndComboState(); //��Ÿ AttackCombo���� �ʱⰪ ����.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));//���� ���� �ݸ��� ������ ���.

	//���ݹ��� ����
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.

	bCanAttackSmallMove = false;
	ExpectedAttackLocation = FVector::ZeroVector;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetController())
	{
		GetController()->SetControlRotation(FRotator(-15.0f, 0.0f, 0.0f)); //��Ʈ�� ȸ�� �⺻�� ����.
	}
	
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//�� ��������
	if (FMath::Abs(SpringArm->TargetArmLength - ExpectedSpringArmLength) > KINDA_SMALL_NUMBER)
	{
		SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, ExpectedSpringArmLength, 0.05f);
	}

	if (bCanAttackSmallMove)
	{			
		if (FMath::Abs(ExpectedAttackLocation.X - GetActorLocation().X) > 0.1)
		{
			FVector temp = FMath::Lerp(GetActorLocation(), ExpectedAttackLocation, 0.05f);
			SetActorLocation(temp);			
		}
		else
		{
			bCanAttackSmallMove = false;
			ExpectedAttackLocation = FVector::ZeroVector;
		}
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MyAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
	}
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded); //AnimInstance�� ��������Ʈ OnMontageEnded�� My�Լ� ���ε�.


	//OnNextAttackCheck ��������Ʈ�� ���ε��� �Լ��� �����Լ��� �����ϰ� ����.
	//AnimInstance�� ��Ƽ�����Լ����� BroadCast�� ��������Ʈ�� ���� ���� �����Լ��� ����.
	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		bCanNextCombo = false;
		if (bIsComboInputOn) //������ǲ�� ������ ��, �����޺� ����.
		{
			AttackStartComboState();//bCanNextCombo true�� �ٲ��ְ�, CurrentCombo+1���ִ� �Լ�.
			MyAnim->JumpToAttackMontageSection(CurrentCombo);//���� �޺��� ��Ÿ�ּ��� ���.
		}
		});
	
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck); //MyAnim���� ���� ��������Ʈ�� MyCharacter�Լ� ���ε�.

}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 


	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		MyAnim->SetDeadAnim();//�״¾ִϸ��̼� ���.
		SetActorEnableCollision(false); //������ �浹�̺�Ʈ �߻����ϰ�.
	}

	return FinalDamage;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ������ �Է� ������ Pawn�� �Լ��� Binding��Ű��.
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown); //TEXT ���� ���� �Է� ������ �̸��̴�.
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);//ĳ���� �����Ʈ ������Ʈ�� ����Ǿ��ִ�.
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AMyCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AMyCharacter::ZoomOut);


}

void AMyCharacter::UpDown(float NewAxisValue)
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
}

void AMyCharacter::LeftRight(float NewAxisValue) //-1,1
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//�¿�� ���̵�.
}
 void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue); 
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);//���콺 �Է� ��ȣ ���� PlayerController�� ControlRotation������ ��ȯ�ϴ� ���.
}

void AMyCharacter::ZoomIn()
{
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength - 150.0f, 200.0f, 800.0f);
}

void AMyCharacter::ZoomOut()
{
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength + 150.0f, 200.0f, 800.0f);
}

void AMyCharacter::Attack()
{
	if (bIsAttacking)//���� �������̸�,
	{
		if(bCanNextCombo) //�����޺��� ������ �� �ִٸ�
		{
			bIsComboInputOn = true; //�޺���ǲ �Է¿��θ� true�� �ٲ��ش�.
		}
	}
	else//���� �������� �ƴϸ�,
	{
		AttackStartComboState();//���� �޺��� ���డ���ϰ� �ϰ�, �޺�+1���ִ� �Լ�
		MyAnim->PlayAttackMontage(); //���� ��Ÿ�� ���.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current�޺��� ��Ÿ�� ���� ���.
		bIsAttacking = true;//���������� �˸�.
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//AnimInstance���� �����ϴ� OnMontageEnded��������Ʈ�� ���ε��� �Լ�.
{
	bIsAttacking = false;//������ �������� �˸�.
	AttackEndComboState();//�޺� �ʱ�ȭ, �����ʱ�ȭ.
}

void AMyCharacter::AttackStartComboState()
{
	bCanAttackSmallMove = true; //�̼����� ����
	ExpectedAttackLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;//������ų ��ǥ��ġ ����.

	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1�� 1�� MaxCombo������ ������ ����� �ʰ� �Ѵ�.
}

void AMyCharacter::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //�޺����� ��, �޺� 0���� �ʱ�ȭ.
}

void AMyCharacter::AttackCheck() //OnAttackCheck ��������Ʈ���� ȣ���� �Լ�.
{
	FHitResult HitResult; //�浹��� ���� ���� ���� ����ü.
	FName temp = NAME_None; 
	FCollisionQueryParams Params(NAME_None, false, this);//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü.
	/*
	* ù��° ���� (TraceTag) : Trace ������� ���� �߰� ���� �Ǵ� ���͸��� �����ϴ� �� ���Ǵ� �±�(��: Collision Analyzer)
	* �ι�° ���� (bTraceComplex) : ������ �浹�� ���� �����ؾ� �ϴ��� ����.
	* ����° ���� (IgnoreActor) : Trace�ϴ� ���� �����ؾ� �ϴ� ����.
	*/

	bool bResult = GetWorld()->SweepSingleByChannel( //Ʈ���̽� ü���� ����� ������ �浹���θ� ������ �Լ�.
		HitResult,
		GetActorLocation(),//Ž��������ġ.
		GetActorLocation() + GetActorForwardVector() * AttackRange,//Ž�� ���� ��ġ.
		FQuat::Identity,//Ž���� ����� ������ ȸ��.
		ECollisionChannel::ECC_GameTraceChannel2,//���� �浹 ������ ����� Ʈ���̽� ä�� ����.
		FCollisionShape::MakeSphere(AttackRadius),//Ž���� ����� �⺻ ���� ����.(��ü,ĸ��,�ڽ� ��)
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//ĸ���� Z���͸� ĳ���� �ü��������� ȸ��.
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule //DrawDebugHelpers���� �����ϴ� ĸ���׸��� �Լ�.
	(
		GetWorld(),//�׸�����
		Center,//��ġ
		HalfHeight,//ĸ������
		AttackRadius,//������
		CapsuleRot,//ĸ��ȸ��
		DrawColor, //����
		false,//���ӿ���
		DebugLifeTime //���ӽð�
	);
#endif

	if (bResult)//�浹�� �����Ǹ�
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);//AActor���� �����ϴ� �Լ�. (������ ������ ����, ����������, ������, �������ظ����� Actor)
		}
	}
}


