// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Boss/MyBoss.h"
#include "Components/CapsuleComponent.h"
#include "AI/MyBossAIController.h"
#include "Animation/MyBossAnimInstance.h"
#include "DrawDebugHelpers.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/Player/MyPlayerController.h"
#include "Components/LockOnComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Inventory/MyWeapon.h"
#include "Kismet/GameplayStatics.h"



const float AMyBoss::MaxHP(100.0f);

// Sets default values
AMyBoss::AMyBoss() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	//무기 오브젝트 만들고 애셋 적용하기
	BossWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/Animations/Boss/SK_BossWeapon.SK_BossWeapon'"));
	if (SK_WEAPON.Succeeded())
	{
		BossWeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	}
	BossWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BossWeaponMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 3.0f));
	



	CurrentHP = MaxHP;

	//스켈레탈 메쉬 에셋 불러오기.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_BOSS(TEXT("SkeletalMesh'/Game/Animations/Boss/SM_Boss.SM_Boss'"));
	if (SM_BOSS.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_BOSS.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); //애니메이션 모드 애님블루프린트 모드로 설정. 
	GetMesh()->SetRelativeLocation(FVector(0.0f, -10.0f, -150.0f)); //메쉬 위치값 맞추기.
	GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); //메쉬 스케일값 조정하기.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -147.0f), FRotator(0.0, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.0f); //캡슐 크기 조정.
	GetCapsuleComponent()->SetCapsuleRadius(60.0f); // 캡슐 너비 조정.
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); //캡슐 스케일값 조정.


	//AnimBP 클래스정보 가져오기.
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Boss(TEXT("AnimBlueprint'/Game/Animations/Boss/AnimBP_Boss.AnimBP_Boss_C'"));
	if (ABP_Boss.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Boss.Class);
	}

	//AI 생성 옵션 설정 : 레벨에 배치하거나 새롭게 생성되는 MyBoss 객체마다 MyBossAIController 생성.
	AIControllerClass = AMyBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster")); // 내가 만든 콜리전 프리셋 사용.


	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LOCKON")); //락온 컴포넌트 생성.


	//파티클 에셋 불러온 후 적용
	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));//이펙트 파티클 설정.
	DeadEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DEADEFFECT(TEXT("ParticleSystem'/Game/FX/Effect_BossDead1.Effect_BossDead1'"));
	if (P_DEADEFFECT.Succeeded())
	{
		DeadEffect->SetTemplate(P_DEADEFFECT.Object);
		DeadEffect->bAutoActivate = false;
	}
	DeadEffect->SetRelativeScale3D(FVector(2.0, 2.0, 2.0));
	DeadEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	//파티클 에셋 불러온 후 적용
	JumpAttackEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("JUMP_ATTACK_EFFECT"));//이펙트 파티클 설정.
	JumpAttackEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_JUMP_ATTACK_EFFECT(TEXT("ParticleSystem'/Game/FX/Effect_JumpAttack.Effect_JumpAttack'"));
	if (P_JUMP_ATTACK_EFFECT.Succeeded())
	{
		JumpAttackEffect->SetTemplate(P_JUMP_ATTACK_EFFECT.Object);
		JumpAttackEffect->bAutoActivate = false;
	}
	JumpAttackEffect->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	JumpAttackEffect->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));
	JumpAttackEffect->SetRelativeRotation(FRotator(0.0f, 0.0f, 200.0f));


	bIsAttackEnded = true;
	BossState = EBossState::NORMALL;
}

void AMyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 보스 AnimInstance 찾아서 멤버변수에 저장.
	BossAnim = Cast<UMyBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == BossAnim) return;

	BossAnim->OnBossAttackEnd().AddLambda([this]()->void {
		bIsAttackEnded = true; //보스 공격이 끝났음으로 설정.
		BossState = EBossState::ATTACKING_END;
		//UE_LOG(LogTemp, Error, TEXT("MyBoss.cpp -----> BossAnim->OnBossAttackEnd().AddLambda "));
	});

	BossAnim->OnChangeFlyingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		});

	BossAnim->OnChangeWalkingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		});

	BossAnim->OnExcuteBossJump().AddLambda([this]()->void {
		JumpToTarget(1500.0f); // 플레이어를 향해 점프.
		});


	BossAnim->OnAttackCheck().AddLambda([this]()->void {
		AttackCheck();
		});

	BossAnim->OnEffectActivate().AddLambda([this]()->void {
		//JumpAttackEffect->Activate(true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpAttackEffect->Template, GetMesh()->GetComponentTransform().GetLocation(), GetActorRotation()*-1, FVector(3.5f, 3.5f, 0.8f), true, EPSCPoolMethod::None, true);
		});

	DeadEffect->OnSystemFinished.AddDynamic(this, &AMyBoss::OnEffectFinished);
}

// Called when the game starts or when spawned
void AMyBoss::BeginPlay()
{
	Super::BeginPlay();
	
	//AIController 받아오기.
	BossAIController = Cast<AMyBossAIController>(GetController());
	if (BossAIController)
	{	
		OnBossHPIsZeroEvent.AddLambda([this]()->void {
			BossAIController->OnAIStop().Broadcast();
			});
	}

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		//보스전 시네마틱 시작했을 때 델리게이트.
		MyPlayerController->OnLevelSequenceStart().AddLambda([this]()->void {
			SetActorHiddenInGame(true);
			});
		
		//보스전 시작했을 때 델리게이트/
		MyPlayerController->OnBossFightStart().AddLambda([this](AMyBoss* _MyBoss)->void {
			SetActorHiddenInGame(false);
			BossAIController->OnAIStart().Broadcast();
			});
	}
	//BossWidget->BindWidgetToBoss(this);

	SetActorHiddenInGame(true);
	BossAIController->OnAIStop().Broadcast();
	
	
	//제작한 무기 클래스 캐릭터에 부착.
	FName WeaponSocket(TEXT("Socket_Weapon"));
	if (nullptr != BossWeaponMesh)
	{
		BossWeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}

// Called every frame
void AMyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

float AMyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.0f, MaxHP); //최근 HP에서 데미지 받은 만큼 빼기.

	if (0 == CurrentHP) //체력이 0으로 떨어지면 이벤트 발생시키기.
	{
		OnBossHPIsZeroEvent.Broadcast();
		DeadEffect->Activate(true);
	}
	else if (2 * CurrentHP < MaxHP && 1==Phase)//체력이 50퍼센트 이하로 떨어지면,
	{
		Phase = 2;//2페이즈로 진입.
		OnPhaseChanged.Broadcast();
	}

	SetHP(CurrentHP);
	return 0.0f;
}

void AMyBoss::SetHP(const float& _NewHP) 
{ 
	CurrentHP = _NewHP; 
	BossHPChangedEvent.Broadcast(CurrentHP / MaxHP); // 이벤트로 현재 HP 비율 전달. 

}

void AMyBoss::ExecuteNormalAttack1()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack1();
	}	

	//공격관련데이터 설정.
	bIsAttackEnded = false;

	AttackPower = 10.0f;//공격 파워
	AttackRange = 300.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 200.0f;//공격 감지 구의 반지름(공격 범위)

}

void AMyBoss::ExecuteNormalAttack2()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack2();
	}

	//공격관련데이터 설정.
	bIsAttackEnded = false;

	AttackPower = 10.0f;//공격 파워
	AttackRange = 300.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 200.0f;//공격 감지 구의 반지름(공격 범위)
}

void AMyBoss::ExecuteNormalAttack3()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack3();
	}

	//공격관련데이터 설정.
	bIsAttackEnded = false; // 공격이 끝났는지 변수를 false로 설정.

	AttackPower = 50.0f;//공격 파워
	AttackRange = 100.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 900.0f;//공격 감지 구의 반지름(공격 범위)
	
}

void AMyBoss::ExecuteStab()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayStab();
	}

	//공격관련데이터 설정.
	bIsAttackEnded = false;

	AttackPower = 20.0f;//공격 파워
	AttackRange = 600.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 100.0f;//공격 감지 구의 반지름(공격 범위)
}

void AMyBoss::ExecuteKick()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayKick();
	}

	//공격관련데이터 설정.
	bIsAttackEnded = false;

	AttackPower = 10.0f;//공격 파워
	AttackRange = 200.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 200.0f;//공격 감지 구의 반지름(공격 범위)
}

void AMyBoss::ExecuteComboAttack()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayComboAttack();
	}

	//공격관련데이터 설정.
	bIsAttackEnded = false;

	AttackPower = 20.0f;//공격 파워
	AttackRange = 300.0f;//공격 감지 구가 지나갈 길이 (공격길이)
	AttackRadius = 200.0f;//공격 감지 구의 반지름(공격 범위)
}


void AMyBoss::JumpToTarget(float _JumpHeight)
{
	auto TargetToJump = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()); //점프할 목표 캐릭터 받아오기.
	auto JumpDestinationVec = TargetToJump->Camera->GetForwardVector(); //캐릭터의 카메라 위치벡터 받아오기.
	
	// 점프 방향 벡터 계산.
	JumpDestinationVec.X *= -1500.0f;
	JumpDestinationVec.Y *= -1500.0f;
	JumpDestinationVec.Z = _JumpHeight;

	LaunchCharacter(JumpDestinationVec, false, false); // 점프 실행.
}

void AMyBoss::AttackCheck() //OnAttackCheck 델리게이트에서 호출할 함수.
{
	FHitResult HitResult; //충돌경우 관련 정보 담을 구조체.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//탐색 방법에 대한 설정 값을 모아둔 구조체.
	FVector StartLocation = GetActorLocation() + GetActorUpVector() * -130.0f +GetActorForwardVector()*100.0f; //충돌처리 시작위치.
	FVector TraceVec = GetActorForwardVector() * AttackRange; // 구를 이동할 최종 위치벡터.
	/*
	* 첫번째 인자 (TraceTag) : Trace 디버깅을 위한 추가 정보 또는 필터링을 제공하는 데 사용되는 태그(예: Collision Analyzer)
	* 두번째 인자 (bTraceComplex) : 복잡한 충돌에 대해 추적해야 하는지 여부.
	* 세번째 인자 (IgnoreActor) : Trace하는 동안 무시해야 하는 엑터.
	*/

	bool bResult = GetWorld()->SweepSingleByChannel( //트레이스 체널을 사용해 물리적 충돌여부를 가리는 함수.
		HitResult,
		StartLocation,//탐색시작위치.
		StartLocation + TraceVec,//탐색 종료 위치.
		FQuat::Identity,//탐색에 사용할 도형의 회전.
		ECollisionChannel::ECC_GameTraceChannel2,//물리 충돌 감지에 사용할 트레이스 채널 정보.
		FCollisionShape::MakeSphere(AttackRadius),//탐색에 사용할 기본 도형 정보.(구체,캡슐,박스 등)
		Params
	);

//#if ENABLE_DRAW_DEBUG
//	FVector Center = StartLocation + TraceVec * 0.5f; //캡슐의 중앙에 해당하는 값.
//	float HalfHeight = AttackRange * 0.5f + AttackRadius; // 캡슐 길이.
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//캡슐의 Z벡터를 캐릭터 시선방향으로 회전.
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule //DrawDebugHelpers에서 제공하는 캡슐그리기 함수.
//	(
//		GetWorld(),//그릴월드
//		Center,//위치
//		HalfHeight,//캡슐길이
//		AttackRadius,//반지름
//		CapsuleRot,//캡슐회전
//		DrawColor, //색깔
//		false,//지속여부
//		DebugLifeTime //지속시간
//	);
//#endif

	if (bResult)//충돌이 감지되면
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(AttackPower, DamageEvent, GetController(), this);//AActor에서 제공하는 함수. (전달할 데미지 세기, 데미지종류, 가해자, 직접피해를입힌 Actor)
		}
	}
}

void AMyBoss::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyPlayerController)
	{
		MyPlayerController->OnBossFightEnd().Broadcast();
	}
	Destroy();
}




