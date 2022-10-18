// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"

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
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


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
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
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

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

}

void AMyCharacter::UpDown(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
}

void AMyCharacter::LeftRight(float NewAxisValue)
{
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
	if (bIsAttacking || CurrentCombo > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("OnAttackMontageEnded Error"));
	}
	bIsAttacking = false;//������ �������� �˸�.
	AttackEndComboState();//�޺� �ʱ�ȭ, �����ʱ�ȭ.
}

void AMyCharacter::AttackStartComboState()
{
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

