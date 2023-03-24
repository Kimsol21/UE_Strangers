// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Animation/MyAnimInstance.h"
#include "DrawDebugHelpers.h" 


// Sets default values
AMyCharacter::AMyCharacter()
{
	//���ݹ��� ����
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) //�����ڿ��� ȣ��� �Լ�.
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	SetDamage(FinalDamage);

	return FinalDamage;
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
			HitResult.Actor->TakeDamage(GetAttackPower(), DamageEvent, GetController(), this);//AActor���� �����ϴ� �Լ�. (������ ������ ����, ����������, ������, �������ظ����� Actor)
		}
	}
}

void AMyCharacter::SetDamage(float _Damage)
{
	return;
}

float AMyCharacter::GetAttackPower()
{
	return 0.0f;
}

/*#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "MyWeapon.h"
#include "MyCharacterStatComponent.h"
#include "DrawDebugHelpers.h" 
#include "Components/WidgetComponent.h"
#include "MyMonsterWidget.h"
#include "MyAIController.h"



// Sets default values
AMyCharacter::AMyCharacter()
{
	/*
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�𸮾� ������Ʈ ���� �� ���λ��� ����.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	//HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	//HPBarWidget->SetupAttachment(GetMesh());

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
	AttackMoveImpulse = 1000.0f;//�������� ���� �ʱ�ȭ.
	bIsAttacking = false;//�������� �ƴ����� �ʱ�ȭ.
	MaxCombo = 4; //�ִ� �޺� ����.
	AttackEndComboState(); //��Ÿ AttackCombo���� �ʱⰪ ����.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));//���� ���� �ݸ��� ������ ���.

	//���ݹ��� ����
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.

	//���� ������Ʈ ��ġ ���� & ������ �������Ʈ �ּ��� Ŭ���� ������ ���� ������Ʈ�� widgetClass�� ���.
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//�׻� �÷��̾� �ٶ󺸱�.
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	
	//AI ���� �ɼ� ���� : ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� MyCharacter��ü���� MyAIController���� ����.
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//���ݹ��� ����
AttackRange = 200.0f; //���� ������ ����.
AttackRadius = 50.0f;//�� ������.
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetController())
	{
		GetController()->SetControlRotation(FRotator(-15.0f, 0.0f, 0.0f)); //��Ʈ�� ȸ�� �⺻�� ����.
	}

	//������ ���� Ŭ���� ĳ���Ϳ� ����.
	FName WeaponSocket(TEXT("FX_weapon_base"));
	auto CurWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		SetWeapon(CurWeapon);
	}

	//ĳ���� ���� �޾ƿ���.
	/*auto CharacterWidget = Cast<UMyMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	
}

bool AMyCharacter::HasAnyWeapon()
{
	return (nullptr != CurrentWeapon); //ĳ���Ϳ� ���Ⱑ �ִ��� �Ǵ� -> ������ true ��ȯ
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon) //ĳ���Ϳ� ���� �����ϴ� �Լ�
{
	if (nullptr != NewWeapon)
	{
		FName WeaponSocket(TEXT("FX_weapon_base"));
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 2. �̼����� �������� ���� ���� ����.

	//�� ��������
	if (bCanZoom)
	{
		if (FMath::Abs(SpringArm->TargetArmLength - ExpectedSpringArmLength) > KINDA_SMALL_NUMBER)
		{
			SpringArm->TargetArmLength = FMath::Lerp(SpringArm->TargetArmLength, ExpectedSpringArmLength, 0.05f);
		}
		else
		{
			SpringArm->TargetArmLength = ExpectedSpringArmLength;
			bCanZoom = false;
		}
	}

	//�̼�����
	if (bCanAttackMove)
	{
		GetCharacterMovement()->MoveSmooth(GetActorForwardVector(), 2.0f);
	}

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MyAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
		return;
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

	//HP�� Zero�϶� ���� �����Լ� ����, ���ε�. ���
	CharacterStat->OnHPIsZero.AddLambda([this]() ->void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});


}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
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
	bCanZoom = true;
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength - 150.0f, 200.0f, 800.0f);
}

void AMyCharacter::ZoomOut()
{
	bCanZoom = true;
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength + 150.0f, 200.0f, 800.0f);
}

void AMyCharacter::Attack()
{
	if (bIsAttacking)//���� �������� ���¿�����,
	{
		if (bCanNextCombo) //�����޺��� ������ �� �ִٸ�
		{
			bIsComboInputOn = true; //�޺���ǲ �Է¿��θ� true�� �ٲ��ش�. OnAttackCheck ��Ƽ���� �߻��� AttackStartComboState�Լ� ���, (��������Ʈ)
		}
	}
	else//�������� ���°� �ƴϾ��ٸ�,
	{
		AttackStartComboState();//���� �޺��� ���డ���ϰ� �ϰ�, �޺�+1���ִ� �Լ�
		MyAnim->PlayAttackMontage(); //���� ��Ÿ�� ���.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current�޺��� ��Ÿ�� ���� ���.

		bIsAttacking = true;//���������� �˸�.
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//UAnimInstance���� �����ϴ� OnMontageEnded��������Ʈ�� ���ε��� �Լ�.
{
	bIsAttacking = false;//������ �������� �˸�.
	AttackEndComboState();//�޺� �ʱ�ȭ, �����ʱ�ȭ.
	OnAttackEnd.Broadcast();
}

void AMyCharacter::AttackStartComboState()
{
	bCanAttackMove = true;//�̼�����

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
	bCanAttackMove = false;//�̼����� ����

	FHitResult HitResult; //�浹��� ���� ���� ���� ����ü.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü.
	/*
	* ù��° ���� (TraceTag) : Trace ������� ���� �߰� ���� �Ǵ� ���͸��� �����ϴ� �� ���Ǵ� �±�(��: Collision Analyzer)
	* �ι�° ���� (bTraceComplex) : ������ �浹�� ���� �����ؾ� �ϴ��� ����.
	* ����° ���� (IgnoreActor) : Trace�ϴ� ���� �����ؾ� �ϴ� ����.
	

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
			HitResult.Actor->TakeDamage(CharacterStat->GetAttackPower(), DamageEvent, GetController(), this);//AActor���� �����ϴ� �Լ�. (������ ������ ����, ����������, ������, �������ظ����� Actor)
		}
	}
}
*/

