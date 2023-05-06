// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Animation/MyAnimInstance.h"
#include "Inventory/MyWeapon.h"
#include "MyPlayerController.h"
#include "Inventory/Item_Interactable.h"
#include "Inventory/InventoryComponent.h"
#include "Character/Player/MyPlayerStatComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/Monster/MyMonster.h"

#pragma region Init Function

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
	//���̷�Ż�޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_KWANG(TEXT("SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood'"));
	if (SM_KWANG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_KWANG.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//�ִԺ������Ʈ.
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Player/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //Ŭ�������� ��������.
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
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //�޺����� ��, �޺� 0���� �ʱ�ȭ.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));//���� ���� �ݸ��� ������ ���.

	//���� LockOn ���� ���� �ʱ�ȭ.
	bIsLockedOn = false;
	TargettingHeightOffset = 20.0f;
	LockedOnCharacter = nullptr;

	TargetCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox"));
	TargetCollisionBox->SetBoxExtent(FVector(2400.0f, 700.0f, 10.0f)); //�ݸ��� ���� ���� ����.
	//TargetCollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // �ݸ��� ��ġ ����.
	TargetCollisionBox->SetCollisionProfileName(TEXT("OnlyPawnDetacted")); //�ݸ��� ������ ����.
	
	TargetCollisionBox->SetupAttachment(GetCapsuleComponent());


	//���׹̳� ���� ���� �ʱ�ȭ.
	MaxStamina = 100.0f; // ���׹̳� �ִ밪.
	CurrentStamina = MaxStamina; //���� ���׹̳� ��.
	bCanStaminaRecharge = true; // ���׹̳��� �� ���� �� �� �ִ��� ����.
	bIsSprinting = false; // �������� ������ ����.


	//����Ʈ ���
	LevelupEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LEVEL_EFFECT"));//����Ʈ ��ƼŬ ����.
	LevelupEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DAMAGED(TEXT("ParticleSystem'/Game/FX/Particle_LevelUp.Particle_LevelUp'"));
	if (P_DAMAGED.Succeeded())
	{
		LevelupEffect->SetTemplate(P_DAMAGED.Object);
		LevelupEffect->bAutoActivate = false;
	}

	//LevelupEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LevelupEffect->SetRelativeScale3D(FVector(0.6, 0.6, 0.6));

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

	//������ ������ �� ȣ��Ǵ� �Լ�.
	MyAnim->OnAttackEnd.AddLambda([this]()->void {
		bIsAttacking = false;//������ �������� �˸�.
		bDoingSomething = false;
		bIsComboInputOn = false;
		bCanNextCombo = false;
		CurrentCombo = 0; //�޺����� ��, �޺� 0���� �ʱ�ȭ.s
		});

	//�����Ⱑ ������ �� ȣ��Ǵ� �Լ�.
	MyAnim->OnRollEnd.AddLambda([this]()->void {
		bIsInvincible = false;
		bDoingSomething = false;
		bCanMyMove = false;
		MyMoveSpeed = 2.0f;
		bDoingSomething = false;
		});

	//������ �ԱⰡ ������ �� ȣ��Ǵ� �Լ�.
	MyAnim->OnDrinkPotionEnd.AddLambda([this]()->void {
		GetCharacterMovement()->MaxWalkSpeed *=5; //���� �̵��ӵ��� ����.
		bDoingSomething = false; // �ൿ������ ���θ� false�� ����.
		isDoingMovableAction = false;
		bIsDrinkPotion = false;
		});


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

	//HP�� Zero�϶� ���� �����Լ� ����, ���ε�. 
	MyStat->OnHPIsZero.AddLambda([this]() ->void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});

	//ĳ���Ͱ� ���������� ��.
	MyStat->OnLevelUp.AddLambda([this]()->void {
		LevelupEffect->Activate(true);
		});

	//�÷��̾ NPC�� ������ �� ��������Ʈ.
	OnPlayerMeetNPCDelegate.AddLambda([this](AMyNPC* _NPC)->void {
		CurrentNPC = _NPC; 
		bIsPlayerTalking = true;
		
		//DialogueWidget->SetVisibility(ESlateVisibility::Visible);
		//_NPC->GetDialogueManagerComponent();
		});

	OnDialogueFinishedDelegate.AddLambda([this]()->void {
		bIsPlayerTalking = false;
		});

	//���� ������ ������ ������ �̺�Ʈ.
	TargetCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnTargetCollisionBeginOverlap); //�ݸ��� ������ ��������Ʈ�� �Լ� ���ε�.
	TargetCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnTargetCollisionEndOverlap);

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

#pragma endregion

#pragma region Tick Function

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	MoveForward();

	//��ü ��ȣ�ۿ�
	CheckForInteractables();

#pragma region LOGIC About Stamina
	if (bIsSprinting) //�������� ���̶��
	{
		//���׹̳� õõ�� ����. (����)
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, 0.0f, DeltaTime, 0.1f);//FInterpConstantTo : ������ step���� Current���� Target���� float ����.
		
		if (CurrentStamina <= 0.0f) //���׹̳��� ���� ���Ǹ�,
		{
			//���ΰ��Ǿ������� ����, �Լ�, ��������Ʈ ����.
		}
	}
	else
	{
		if (CurrentStamina < MaxStamina) //���׹̳��� �ִ�ġ�� �ƴ϶��,
		{
			if (bCanStaminaRecharge) // ������ �� �ִ� ��Ȳ�϶� ������ ������.
			{
				CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, MaxStamina, DeltaTime, bCanStaminaRecharge);
			}
		}
	}
#pragma endregion 

#pragma region LOGIC About LockON
	if (bIsLockedOn) // ���� ���¶��, 
	{
		//ī�޶� ���� ����.
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOnCharacter->GetActorLocation());
		LookAtRotation.Pitch -= TargettingHeightOffset; // ���� �� ����.
		GetController()->SetControlRotation(LookAtRotation);
	}
#pragma endregion
}

void AMyPlayer::MoveForward()
{
	if (bCanMyMove)
	{
		GetCharacterMovement()->MoveSmooth(GetActorForwardVector(), MyMoveSpeed);
	}
}

void AMyPlayer::OnTargetCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto OverlappedCharacter= Cast<ACharacter>(OtherActor);
	if (OverlappedCharacter)
	{
		//���� ������ �����ȿ� ������ ������ �迭�� �߰�.
		LockOnCandidates.AddUnique(OverlappedCharacter); //AddUnique : ���� �����̳ʿ� ������ ������Ʈ�� �̹� �����ϴ� ���� �߰�����.  
		if (AMyMonster* Monster = Cast<AMyMonster>(OverlappedCharacter))
		{
			Monster->OnMonsterMeetPlayer().Broadcast();
		}
	}
}

void AMyPlayer::OnTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto OverlappedCharacter = Cast<ACharacter>(OtherActor);
	if (OverlappedCharacter)
	{
		//�������� ����� ���� �����ڿ��� Ż��.
		LockOnCandidates.Remove(OverlappedCharacter); 
		if (AMyMonster* Monster = Cast<AMyMonster>(OverlappedCharacter))
		{
			Monster->OnMonsterFartherAwayPlayer().Broadcast(); 
		}
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
	if (MyPlayerController)
	{
		if (result)
		{
			if (AItem_Interactable* Interactable = Cast<AItem_Interactable>(HitResult.GetActor()))
			{
				MyPlayerController->SetCurrentInteractableItem(Interactable);
				return;
			}
		}
		MyPlayerController->SetCurrentInteractableItem(nullptr);
	}
}


#pragma endregion

#pragma region Input Binding Function

void AMyPlayer::UpDown(float NewAxisValue)
{
	if (!bDoingSomething)//������ �ϰ����� �������� �̵��� �� �ִ�.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
	}
	else if (isDoingMovableAction)//������ �ϸ鼭 �̵��Ҽ� �ִ� ������ ��쿡�� �̵��� �� �ְ��Ѵ�.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
	}
}

void AMyPlayer::LeftRight(float NewAxisValue) //-1,1
{
	if (!bDoingSomething)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//�¿�� ���̵�.
	}
	else if (isDoingMovableAction)//������ �ϸ鼭 �̵��Ҽ� �ִ� ������ ��쿡�� �̵��� �� �ְ��Ѵ�.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//�𸮾� �������� �ü������� X��, ���� ������ Y���� �ǹ��Ѵ�.
	}
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

void AMyPlayer::Roll()
{
	if (bDoingSomething || GetCharacterMovement()->IsFalling()) return;

	if (MyAnim)
	{
		MyMoveSpeed = 4.0f; // ������ �ӵ� ����.
		bCanMyMove = true; // Tick���� �� ĳ���Ͱ� ������ �� �ֵ��� ����.
		bIsInvincible = true; // �������·� ��ȯ.
		bDoingSomething = true;
		MyAnim->PlayRollMontage();
		bDoingSomething = true;
	}	
}

void AMyPlayer::DrinkPotion()
{
	if (bDoingSomething || GetCharacterMovement()->IsFalling()) return;

	if (MyAnim)
	{
		bIsDrinkPotion = true;
		
		isDoingMovableAction = true;
		bDoingSomething = true;
		GetCharacterMovement()->MaxWalkSpeed *= 0.2f; //���Ǹ������� ������ �ӵ� ����.
		MyAnim->PlayDrinkPotion();
		OnStartDrinkPotion.Broadcast();
		//���ǸԱ� �ִϸ��̼� ���. 
		//��Ƽ���� ��ε��ɽ�Ʈ.	
	}
}

void AMyPlayer::LockOn()
{
	if (bIsLockedOn) // �̹� �������� ���¿��ٸ�
	{
		bIsLockedOn = false; //���� ����.


		if (AMyMonster* MyMonster = Cast<AMyMonster>(LockedOnCharacter))
		{
			MyMonster->OnLockOnRemoveThis().Broadcast();
		}

		LockedOnCharacter = nullptr; // ���µǾ��ִ� ĳ���� nulló��.
	}
	else
	{
		if (LockOnCandidates.Num() > 0) // �ֺ��� ���� ����ڰ� �����Ǿ����� Ȯ��.
		{
			LockedOnCharacter = LockOnCandidates[0]; //����� �� ���� ����� ����� ����ĳ���Ϳ� ����.

			if (LockedOnCharacter) // ����ĳ���Ͱ� ��ȿ�ϴٸ�,
			{
				bIsLockedOn = true; // ���������� true�� ����.
				
				//���� �������� ĳ���Ͱ� ���Ͷ��,
				if (AMyMonster* MyMonster = Cast<AMyMonster>(LockedOnCharacter))
				{
					MyMonster->OnPlayerFocusOnThis().Broadcast();
				}
			}
		}
	}
}

#pragma endregion

#pragma region Attack Function

void AMyPlayer::Attack()
{
	if (!bDoingSomething)//������ ���ϰ������� ���ݰ���
	{		
		AttackStartComboState();//���� �޺��� ���డ���ϰ� �ϰ�, �޺�+1���ִ� �Լ�
		MyAnim->PlayAttackMontage(); //���� ��Ÿ�� ���.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current�޺��� ��Ÿ�� ���� ���.

		bIsAttacking = true;//���������� �˸�.
		bDoingSomething = true;
	}
	else
	{
		if (bIsAttacking)//���� �������� ���¿�����,
		{
			if (bCanNextCombo) //�����޺��� ������ �� �ִٸ�
			{
				bIsComboInputOn = true; //�޺���ǲ �Է¿��θ� true�� �ٲ��ش�. OnAttackCheck ��Ƽ���� �߻��� AttackStartComboState�Լ� ���, (��������Ʈ)
			}
		}
	}
}

void AMyPlayer::AttackStartComboState()
{
	bCanMyMove = true;//�̼�����
	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1�� 1�� MaxCombo������ ������ ����� �ʰ� �Ѵ�.
}

void AMyPlayer::AttackCheck()
{
	Super::AttackCheck();

	bCanMyMove = false;//�̼����� ����
}

float AMyPlayer::GetAttackPower()
{
	return MyStat->GetAttackPower();
}

void AMyPlayer::SetDamage(float _Damage)
{
	MyStat->SetDamage(_Damage);
}

float AMyPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	MyStat->SetDamage(FinalDamage); //������ ����.


	//bDoingSomething = true;
	MyAnim->PlayDamagedMontage();

	return FinalDamage;
}

#pragma endregion

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

bool AMyPlayer::HasAnyWeapon()
{
	return (nullptr != CurrentWeapon); //ĳ���Ϳ� ���Ⱑ �ִ��� �Ǵ� -> ������ true ��ȯ
}

void AMyPlayer::SetEXP(float _NewEXP)
{
	MyStat->SetEXP(_NewEXP);
}
