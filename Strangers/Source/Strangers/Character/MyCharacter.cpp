// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Animation/MyAnimInstance.h"
#include "DrawDebugHelpers.h" 


// Sets default values
AMyCharacter::AMyCharacter()
{
	//공격범위 관련
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) //피해자에게 호출될 함수.
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	SetDamage(FinalDamage);

	return FinalDamage;
}

void AMyCharacter::AttackCheck() //OnAttackCheck 델리게이트에서 호출할 함수.
{
	FHitResult HitResult; //충돌경우 관련 정보 담을 구조체.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//탐색 방법에 대한 설정 값을 모아둔 구조체.
	/*
	* 첫번째 인자 (TraceTag) : Trace 디버깅을 위한 추가 정보 또는 필터링을 제공하는 데 사용되는 태그(예: Collision Analyzer)
	* 두번째 인자 (bTraceComplex) : 복잡한 충돌에 대해 추적해야 하는지 여부.
	* 세번째 인자 (IgnoreActor) : Trace하는 동안 무시해야 하는 엑터.
	*/

	bool bResult = GetWorld()->SweepSingleByChannel( //트레이스 체널을 사용해 물리적 충돌여부를 가리는 함수.
		HitResult,
		GetActorLocation(),//탐색시작위치.
		GetActorLocation() + GetActorForwardVector() * AttackRange,//탐색 종료 위치.
		FQuat::Identity,//탐색에 사용할 도형의 회전.
		ECollisionChannel::ECC_GameTraceChannel2,//물리 충돌 감지에 사용할 트레이스 채널 정보.
		FCollisionShape::MakeSphere(AttackRadius),//탐색에 사용할 기본 도형 정보.(구체,캡슐,박스 등)
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//캡슐의 Z벡터를 캐릭터 시선방향으로 회전.
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule //DrawDebugHelpers에서 제공하는 캡슐그리기 함수.
	(
		GetWorld(),//그릴월드
		Center,//위치
		HalfHeight,//캡슐길이
		AttackRadius,//반지름
		CapsuleRot,//캡슐회전
		DrawColor, //색깔
		false,//지속여부
		DebugLifeTime //지속시간
	);
#endif

	if (bResult)//충돌이 감지되면
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(GetAttackPower(), DamageEvent, GetController(), this);//AActor에서 제공하는 함수. (전달할 데미지 세기, 데미지종류, 가해자, 직접피해를입힌 Actor)
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

	//언리얼 오브젝트 생성 및 세부사항 설정.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UMyCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	//HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	//HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, -90.0f, 0.0f));

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

	//공격범위 관련
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.

	//위젯 컴포넌트 위치 조정 & 제작한 블루프린트 애셋의 클래스 정보를 위젯 컴포넌트의 widgetClass로 등록.
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//항상 플레이어 바라보기.
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	
	//AI 생성 옵션 설정 : 레벨에 배치하거나 새롭게 생성되는 MyCharacter객체마다 MyAIController액터 생성.
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//공격범위 관련
AttackRange = 200.0f; //구가 지나갈 길이.
AttackRadius = 50.0f;//구 반지름.
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetController())
	{
		GetController()->SetControlRotation(FRotator(-15.0f, 0.0f, 0.0f)); //컨트롤 회전 기본값 지정.
	}

	//제작한 무기 클래스 캐릭터에 부착.
	FName WeaponSocket(TEXT("FX_weapon_base"));
	auto CurWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		SetWeapon(CurWeapon);
	}

	//캐릭터 위젯 받아오기.
	/*auto CharacterWidget = Cast<UMyMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	
}

bool AMyCharacter::HasAnyWeapon()
{
	return (nullptr != CurrentWeapon); //캐릭터에 무기가 있는지 판단 -> 없으면 true 반환
}

void AMyCharacter::SetWeapon(AMyWeapon* NewWeapon) //캐릭터에 무기 장착하는 함수
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
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded); //AnimInstance의 델리게이트 OnMontageEnded에 My함수 바인딩.


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

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck); //MyAnim에서 만든 델리게이트에 MyCharacter함수 바인딩.

	//HP가 Zero일때 관련 람다함수 선언, 바인딩. 김솔
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

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//언리얼 엔진에서 시선방향은 X축, 우측 방향은 Y축을 의미한다.
}

void AMyCharacter::LeftRight(float NewAxisValue) //-1,1
{
	if (bIsAttacking) return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//좌우로 폰이동.
}
void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);//마우스 입력 신호 값을 PlayerController의 ControlRotation값으로 변환하는 명령.
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

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)//UAnimInstance에서 제공하는 OnMontageEnded델리게이트에 바인딩된 함수.
{
	bIsAttacking = false;//공격이 끝났음을 알림.
	AttackEndComboState();//콤보 초기화, 변수초기화.
	OnAttackEnd.Broadcast();
}

void AMyCharacter::AttackStartComboState()
{
	bCanAttackMove = true;//미세전진

	bCanNextCombo = true;
	bIsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo); //CurrentCombo+1이 1과 MaxCombo까지의 범위를 벗어나지 않게 한다.
}

void AMyCharacter::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0; //콤보공격 끝, 콤보 0으로 초기화.
}

void AMyCharacter::AttackCheck() //OnAttackCheck 델리게이트에서 호출할 함수.
{
	bCanAttackMove = false;//미세전진 중지

	FHitResult HitResult; //충돌경우 관련 정보 담을 구조체.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//탐색 방법에 대한 설정 값을 모아둔 구조체.
	/*
	* 첫번째 인자 (TraceTag) : Trace 디버깅을 위한 추가 정보 또는 필터링을 제공하는 데 사용되는 태그(예: Collision Analyzer)
	* 두번째 인자 (bTraceComplex) : 복잡한 충돌에 대해 추적해야 하는지 여부.
	* 세번째 인자 (IgnoreActor) : Trace하는 동안 무시해야 하는 엑터.
	

	bool bResult = GetWorld()->SweepSingleByChannel( //트레이스 체널을 사용해 물리적 충돌여부를 가리는 함수.
		HitResult,
		GetActorLocation(),//탐색시작위치.
		GetActorLocation() + GetActorForwardVector() * AttackRange,//탐색 종료 위치.
		FQuat::Identity,//탐색에 사용할 도형의 회전.
		ECollisionChannel::ECC_GameTraceChannel2,//물리 충돌 감지에 사용할 트레이스 채널 정보.
		FCollisionShape::MakeSphere(AttackRadius),//탐색에 사용할 기본 도형 정보.(구체,캡슐,박스 등)
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//캡슐의 Z벡터를 캐릭터 시선방향으로 회전.
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule //DrawDebugHelpers에서 제공하는 캡슐그리기 함수.
	(
		GetWorld(),//그릴월드
		Center,//위치
		HalfHeight,//캡슐길이
		AttackRadius,//반지름
		CapsuleRot,//캡슐회전
		DrawColor, //색깔
		false,//지속여부
		DebugLifeTime //지속시간
	);
#endif

	if (bResult)//충돌이 감지되면
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttackPower(), DamageEvent, GetController(), this);//AActor에서 제공하는 함수. (전달할 데미지 세기, 데미지종류, 가해자, 직접피해를입힌 Actor)
		}
	}
}
*/

