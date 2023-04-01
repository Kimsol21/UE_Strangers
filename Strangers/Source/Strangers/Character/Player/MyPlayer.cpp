// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Animation/MyAnimInstance.h"
#include "Inventory/MyWeapon.h"
#include "MyPlayerController.h"
#include "Inventory/Item_Interactable.h"
#include "Inventory/InventoryComponent.h"
#include "Character/Player/MyPlayerStatComponent.h"

AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	


	//�𸮾� ������Ʈ ���� �� ���λ��� ����.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	MyStat = CreateDefaultSubobject<UMyPlayerStatComponent>(TEXT("CHARACTERSTAT"));
	MyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("INVENTORY"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, -90.0f, 0.0f));
	Camera->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));

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

}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(GetController());

	if (MyPlayerController)
	{
		MyPlayerController->SetControlRotation(FRotator(-15.0f, 0.0f, 0.0f)); //��Ʈ�� ȸ�� �⺻�� ����. (�ʱ� ī�޶� ���� ����)
	}

	//������ ���� Ŭ���� ĳ���Ϳ� ����.
	FName WeaponSocket(TEXT("FX_weapon_base"));
	auto CurWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		SetWeapon(CurWeapon);
	}
}

void AMyPlayer::AttackCheck()
{
	Super::AttackCheck();
	bCanAttackMove = false;//�̼����� ����
}

void AMyPlayer::SetDamage(float _Damage)
{
	MyStat->SetDamage(_Damage);
}

float AMyPlayer::GetAttackPower()
{
	return MyStat->GetAttackPower();
}

bool AMyPlayer::HasAnyWeapon()
{
	return (nullptr != CurrentWeapon); //ĳ���Ϳ� ���Ⱑ �ִ��� �Ǵ� -> ������ true ��ȯ
}

void AMyPlayer::SetWeapon(AMyWeapon* NewWeapon) //ĳ���Ϳ� ���� �����ϴ� �Լ�
{
	if (nullptr != NewWeapon)
	{
		FName WeaponSocket(TEXT("FX_weapon_base"));
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}


void AMyPlayer::SetEXP(float _NewEXP)
{
	MyStat->SetEXP(_NewEXP);
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
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


	//��ü ��ȣ�ۿ�
	CheckForInteractables();

}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MyAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
		return;
	}
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded); //AnimInstance�� ��������Ʈ OnMontageEnded�� My�Լ� ���ε�.


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

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyPlayer::AttackCheck); //MyAnim���� ���� ��������Ʈ�� MyCharacter�Լ� ���ε�.

	//HP�� Zero�϶� ���� �����Լ� ����, ���ε�. ���
	MyStat->OnHPIsZero.AddLambda([this]() ->void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});


	
}

float AMyPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MyStat->SetDamage(FinalDamage);
	MyAnim->PlayDamagedMontage();

	return FinalDamage;
}

void AMyPlayer::UpDown(float NewAxisValue)
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
}

void AMyPlayer::LeftRight(float NewAxisValue) //-1,1
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//�¿�� ���̵�.
}
void AMyPlayer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyPlayer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);//���콺 �Է� ��ȣ ���� PlayerController�� ControlRotation������ ��ȯ�ϴ� ���.
}

void AMyPlayer::ZoomIn()
{
	bCanZoom = true;
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength - 150.0f, 200.0f, 800.0f);
}

void AMyPlayer::ZoomOut()
{
	bCanZoom = true;
	ExpectedSpringArmLength = FMath::Clamp<float>(ExpectedSpringArmLength + 150.0f, 200.0f, 800.0f);
}

void AMyPlayer::Attack()
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

void AMyPlayer::CheckForInteractables()
{
	FHitResult HitResult;

	FVector StartTrace = Camera->GetComponentLocation();
	FVector EndTrace = (Camera->GetForwardVector() * 800) + StartTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	
	bool result = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams);
	if (MyPlayerController && result)
	{
		
		if (AItem_Interactable* Interactable = Cast<AItem_Interactable>(HitResult.GetActor()))
		{
			MyPlayerController->SetCurrentInteractableItem(Interactable);
			return;
		}
	}
	MyPlayerController->SetCurrentInteractableItem(nullptr);
	
}

void AMyPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//UAnimInstance���� �����ϴ� OnMontageEnded��������Ʈ�� ���ε��� �Լ�.
{
	bIsAttacking = false;//������ �������� �˸�.
	AttackEndComboState();//�޺� �ʱ�ȭ, �����ʱ�ȭ.
	OnAttackEnd.Broadcast();
}

void AMyPlayer::AttackStartComboState()
{
	bCanAttackMove = true;//�̼�����

	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1�� 1�� MaxCombo������ ������ ����� �ʰ� �Ѵ�.
}

void AMyPlayer::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //�޺����� ��, �޺� 0���� �ʱ�ȭ.
}