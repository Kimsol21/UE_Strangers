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

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C'")); //클래스정보 가져오기.
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
	AttackEndComboState(); //기타 AttackCombo관련 초기값 지정.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));//내가 만든 콜리전 프리셋 사용.

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

void AMyPlayer::AttackCheck()
{
	Super::AttackCheck();
	bCanAttackMove = false;//미세전진 중지
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
	return (nullptr != CurrentWeapon); //캐릭터에 무기가 있는지 판단 -> 없으면 true 반환
}

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


void AMyPlayer::SetEXP(float _NewEXP)
{
	MyStat->SetEXP(_NewEXP);
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 2. 미세전진 점프공격 관련 버그 수정.

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
	if (bCanAttackMove)
	{
		GetCharacterMovement()->MoveSmooth(GetActorForwardVector(), 2.0f);
	}


	//물체 상호작용
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
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded); //AnimInstance의 델리게이트 OnMontageEnded에 My함수 바인딩.


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

	//HP가 Zero일때 관련 람다함수 선언, 바인딩. 김솔
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

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
}

void AMyPlayer::LeftRight(float NewAxisValue) //-1,1
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//좌우로 폰이동.
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

void AMyPlayer::Attack()
{
	if (bIsAttacking)//원래 공격중인 상태였으면,
	{
		if (bCanNextCombo) //다음콤보를 실행할 수 있다면
		{
			bIsComboInputOn = true; //콤보인풋 입력여부를 true로 바꿔준다. OnAttackCheck 노티파이 발생시 AttackStartComboState함수 출력, (델리게이트)
		}
	}
	else//공격중인 상태가 아니었다면,
	{
		AttackStartComboState();//다음 콤보로 진행가능하게 하고, 콤보+1해주는 함수
		MyAnim->PlayAttackMontage(); //공격 몽타주 재생.
		MyAnim->JumpToAttackMontageSection(CurrentCombo);//Current콤보의 몽타주 섹션 재생.

		bIsAttacking = true;//공격중임을 알림.
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

void AMyPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//UAnimInstance에서 제공하는 OnMontageEnded델리게이트에 바인딩된 함수.
{
	bIsAttacking = false;//공격이 끝났음을 알림.
	AttackEndComboState();//콤보 초기화, 변수초기화.
	OnAttackEnd.Broadcast();
}

void AMyPlayer::AttackStartComboState()
{
	bCanAttackMove = true;//미세전진

	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1이 1과 MaxCombo까지의 범위를 벗어나지 않게 한다.
}

void AMyPlayer::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //콤보공격 끝, 콤보 0으로 초기화.
}