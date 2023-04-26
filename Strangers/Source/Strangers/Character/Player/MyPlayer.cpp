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

#pragma region Init Function

AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//언리얼 오브젝트 생성 및 세부사항 설정.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	MyStat = CreateDefaultSubobject<UMyPlayerStatComponent>(TEXT("CHARACTERSTAT"));
	MyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("INVENTORY"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, -90.0f, 0.0f));
	Camera->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));

	//에셋 불러오기.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_KWANG(TEXT("SkeletalMesh'/Game/Assets/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood'"));
	if (SM_KWANG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_KWANG.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Player/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //클래스정보 가져오기.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//GTA식 ControlMode 설정.
	ExpectedSpringArmLength = 450.0f;
	SpringArm->TargetArmLength = ExpectedSpringArmLength;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;//캐릭터가 움직이는 방향으로 캐릭터를 자동 회전시켜주는 기능.
	GetCharacterMovement()->bUseControllerDesiredRotation = false;//컨트롤 회전이 가리키는 방향으로 캐릭터가 부드럽게 회전.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 800.0f; //점프 높이 지정
	AttackMoveImpulse = 1000.0f;//공격전진 세기 초기화.
	bIsAttacking = false;//공격중이 아님으로 초기화.
	MaxCombo = 4; //최대 콤보 지정.
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //콤보공격 끝, 콤보 0으로 초기화.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));//내가 만든 콜리전 프리셋 사용.



	//락온 LockOn 관련 변수 초기화.
	bIsLockedOn = false;
	TargettingHeightOffset = 20.0f;
	LockedOnCharacter = nullptr;

	TargetCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetCollisionBox"));
	TargetCollisionBox->SetBoxExtent(FVector(400.0f, 400.0f, 300.0f)); //콜리전 감지 범위 설정.
	TargetCollisionBox->SetCollisionProfileName(TEXT("OnlyPawnDetacted")); //콜리전 프리셋 설정.
	TargetCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnTargetCollisionBeginOverlap); //콜리전 오버랩 델리게이트에 함수 바인딩.
	TargetCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnTargetCollisionEndOverlap);



	//스테미나 관련 변수 초기화.
	MaxStamina = 100.0f; // 스테미나 최대값.
	CurrentStamina = MaxStamina; //현재 스테미나 값.
	bCanStaminaRecharge = true; // 스테미나가 재 충전 될 수 있는지 여부.
	bIsSprinting = false; // 전력질주 중인지 여부.
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
	//MyAnim->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnMyMontageEnded); //AnimInstance의 델리게이트 OnMontageEnded에 My함수 바인딩.
	MyAnim->OnAttackEnd.AddLambda([this]()->void {
		bIsAttacking = false;//공격이 끝났음을 알림.
		bDoingSomething = false;
		bIsComboInputOn = false;
		bCanNextCombo = false;
		CurrentCombo = 0; //콤보공격 끝, 콤보 0으로 초기화.s
		});

	MyAnim->OnRollEnd.AddLambda([this]()->void {
		bIsInvincible = false;
		bDoingSomething = false;
		bCanMyMove = false;
		MyMoveSpeed = 2.0f;
		bDoingSomething = false;
		});

	MyAnim->OnDrinkPotionEnd.AddLambda([this]()->void {
		GetCharacterMovement()->MaxWalkSpeed *=5; //원래 이동속도로 복귀.
		bDoingSomething = false; // 행동중인지 여부를 false로 설정.
		isDoingMovableAction = false;
		bIsDrinkPotion = false;
		});


	//OnNextAttackCheck 델리게이트에 바인딩할 함수를 람다함수로 간편하게 구현.
	//AnimInstance의 노티파이함수에서 BroadCast한 델리게이트에 의해 다음 람다함수가 실행.
	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		bCanNextCombo = false;
		if (bIsComboInputOn) //공격인풋이 들어왔을 시, 다음콤보 진행.
		{
			AttackStartComboState();//bCanNextCombo true로 바꿔주고, CurrentCombo+1해주는 함수.
			MyAnim->JumpToAttackMontageSection(CurrentCombo);//다음 콤보의 몽타주섹션 재생.
		}
		});

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyPlayer::AttackCheck); //MyAnim에서 만든 델리게이트에 MyCharacter함수 바인딩.

	//HP가 Zero일때 관련 람다함수 선언, 바인딩. 
	MyStat->OnHPIsZero.AddLambda([this]() ->void {
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		});



	

}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(GetController());

	if (MyPlayerController)
	{
		MyPlayerController->SetControlRotation(FRotator(-15.0f, 0.0f, 0.0f)); //컨트롤 회전 기본값 지정. (초기 카메라 각도 조정)
	}

	//제작한 무기 클래스 캐릭터에 부착.
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

	//줌 선형보간
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

	//미세전진
	MoveForward();

	//물체 상호작용
	CheckForInteractables();

#pragma region LOGIC About Stamina
	if (bIsSprinting) //전력질주 중이라면
	{
		//스테미나 천천히 감소. (보간)
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, 0.0f, DeltaTime, 0.1f);//FInterpConstantTo : 일정한 step으로 Current에서 Target까지 float 보간.
		
		if (CurrentStamina <= 0.0f) //스테미나가 전부 고갈되면,
		{
			//전부고갈되었을때의 로직, 함수, 델리게이트 실현.
		}
	}
	else
	{
		if (CurrentStamina < MaxStamina) //스테미나가 최대치가 아니라면,
		{
			if (bCanStaminaRecharge) // 충전할 수 있는 상황일때 서서히 재충전.
			{
				CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, MaxStamina, DeltaTime, bCanStaminaRecharge);
			}
		}
	}
#pragma endregion 

#pragma region LOGIC About LockON
	if (bIsLockedOn) // 락온 상태라면, 
	{
		//카메라 변경 로직.
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOnCharacter->GetActorLocation());
		LookAtRotation.Pitch -= TargettingHeightOffset; // 높이 값 적용.
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
		//락온 가능한 범위안에 들어오면 지원자 배열에 추가.
		LockOnCandidates.AddUnique(OverlappedCharacter); //AddUnique : 기존 컨테이너에 동일한 엘리먼트가 이미 존재하는 경우는 추가안함. 
	}
}

void AMyPlayer::OnTargetCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto OverlappedCharacter = Cast<ACharacter>(OtherActor);
	if (OverlappedCharacter)
	{
		//범위에서 벗어나면 락온 지원자에서 탈락.
		LockOnCandidates.Remove(OverlappedCharacter); 
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


#pragma endregion

#pragma region Input Binding Function

void AMyPlayer::UpDown(float NewAxisValue)
{
	if (!bDoingSomething)//뭔가를 하고있지 않을때만 이동할 수 있다.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
	}
	else if (isDoingMovableAction)//뭔가를 하면서 이동할수 있는 동작인 경우에도 이동할 수 있게한다.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
	}
}

void AMyPlayer::LeftRight(float NewAxisValue) //-1,1
{
	if (!bDoingSomething)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//좌우로 폰이동.
	}
	else if (isDoingMovableAction)//뭔가를 하면서 이동할수 있는 동작인 경우에도 이동할 수 있게한다.
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
	}
}
void AMyPlayer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyPlayer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);//마우스 입력 신호 값을 PlayerController의 ControlRotation값으로 변환하는 명령.
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
		MyMoveSpeed = 4.0f; // 구르기 속도 조절.
		bCanMyMove = true; // Tick에서 내 캐릭터가 움직일 수 있도록 설정.
		bIsInvincible = true; // 무적상태로 전환.
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
		GetCharacterMovement()->MaxWalkSpeed *= 0.2f; //포션먹을때는 움직임 속도 감소.
		MyAnim->PlayDrinkPotion();
		OnStartDrinkPotion.Broadcast();
		//포션먹기 애니메이션 재생. 
		//노티파이 브로드케스트.	
	}
}

void AMyPlayer::LockOn()
{
	if (bIsLockedOn) // 이미 락온중인 상태였다면
	{
		bIsLockedOn = false; //락온 해제.
		LockedOnCharacter = nullptr; // 락온되어있던 캐릭터 null처리.
	}
	else
	{
		if (LockOnCandidates.Num() > 0) // 주변에 락온 희망자가 감지되었는지 확인.
		{
			LockedOnCharacter = LockOnCandidates[0]; //희망자 중 가장 가까운 사람을 락온캐릭터에 저장.

			if (LockedOnCharacter) // 락온캐릭터가 유효하다면,
			{
				bIsLockedOn = true; // 락온중임을 true로 설정.
			}
		}
	}
}

#pragma endregion

#pragma region Attack Function

void AMyPlayer::Attack()
{
	if (!bDoingSomething)//뭔가를 안하고있을때 공격개시
	{		
		AttackStartComboState();//다음 콤보로 진행가능하게 하고, 콤보+1해주는 함수
		MyAnim->PlayAttackMontage(); //공격 몽타주 재생.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current콤보의 몽타주 섹션 재생.

		bIsAttacking = true;//공격중임을 알림.
		bDoingSomething = true;
	}
	else
	{
		if (bIsAttacking)//원래 공격중인 상태였으면,
		{
			if (bCanNextCombo) //다음콤보를 실행할 수 있다면
			{
				bIsComboInputOn = true; //콤보인풋 입력여부를 true로 바꿔준다. OnAttackCheck 노티파이 발생시 AttackStartComboState함수 출력, (델리게이트)
			}
		}
	}
}

void AMyPlayer::AttackStartComboState()
{
	bCanMyMove = true;//미세전진
	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1이 1과 MaxCombo까지의 범위를 벗어나지 않게 한다.
}

void AMyPlayer::AttackCheck()
{
	Super::AttackCheck();

	bCanMyMove = false;//미세전진 중지
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

	MyStat->SetDamage(FinalDamage); //데미지 설정.


	//bDoingSomething = true;
	MyAnim->PlayDamagedMontage();

	return FinalDamage;
}

#pragma endregion

void AMyPlayer::SetWeapon(AMyWeapon* NewWeapon) //캐릭터에 무기 장착하는 함수
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
	return (nullptr != CurrentWeapon); //캐릭터에 무기가 있는지 판단 -> 없으면 true 반환
}

void AMyPlayer::SetEXP(float _NewEXP)
{
	MyStat->SetEXP(_NewEXP);
}

