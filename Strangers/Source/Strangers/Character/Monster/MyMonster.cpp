// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Character/Player/MyPlayer.h"
#include "Components/WidgetComponent.h"
#include "AI/MyAIController.h"
#include "UI/MyMonsterWidget.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/Monster_AnimInstance.h"
#include "Character/Monster/MyMonsterStatComponent.h"
#include "Components/LockOnComponent.h"

AMyMonster::AMyMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET")); //HP바 위젯 컴포넌트 생성.
	HPBarWidget->SetupAttachment(GetMesh());

	MyStat = CreateDefaultSubobject<UMyMonsterStatComponent>(TEXT("MONSTERSTAT"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LOCKON")); //락온 컴포넌트 생성.
	

	//에셋 불러오기.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_CORPSE(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Corpse_Basic/Corpse_Basic.Corpse_Basic'"));
	if (SM_CORPSE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_CORPSE.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Monster/Monster_AnimBlueprint.Monster_AnimBlueprint_C'")); //클래스정보 가져오기.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//위젯 컴포넌트 위치 조정 & 제작한 블루프린트 애셋의 클래스 정보를 위젯 컴포넌트의 widgetClass로 등록.
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//항상 플레이어 바라보기.

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	////락온 Circle UI
	//LockOnWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("LockOnWidget")); //HP바 위젯 컴포넌트 생성.
	//LockOnWidget->SetupAttachment(GetMesh());
	//LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);

	/*static ConstructorHelpers::FClassFinder<UUserWidget> UI_LOCKON(TEXT("WidgetBlueprint'/Game/UI/Monster/WG_LockOn.WG_LockOn_C'"));
	if (UI_LOCKON.Succeeded())
	{
		LockOnWidget->SetWidgetClass(UI_LOCKON.Class);
		LockOnWidget->SetDrawSize(FVector2D(35.0f, 35.0f));
		
	}
	LockOnWidget->SetVisibility(false);
	LockOnWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));*/
	


	//AI 생성 옵션 설정 : 레벨에 배치하거나 새롭게 생성되는 MyMonster객체마다 MyAIController액터 생성.
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));//내가 만든 콜리전 프리셋 사용.

	//파티클 에셋 불러온 후 적용
	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));//이펙트 파티클 설정.
	DeadEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DEADEFFECT(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Archive/P_HealthOrb_Pickup.P_HealthOrb_Pickup'"));
	if (P_DEADEFFECT.Succeeded())
	{
		DeadEffect->SetTemplate(P_DEADEFFECT.Object);
		DeadEffect->bAutoActivate = false;
	}
	DeadEffect->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	DeadEffect->SetRelativeLocation(FVector(-80.0f, 0.0f, 0.0f));

	//Hit Particle
	DamagedEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT2"));//이펙트 파티클 설정.
	DamagedEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DAMAGED(TEXT("ParticleSystem'/Game/FX/Particle_Damaged.Particle_Damaged'"));
	if (P_DAMAGED.Succeeded())
	{
		DamagedEffect->SetTemplate(P_DAMAGED.Object);
		DamagedEffect->bAutoActivate = false;
	}

	DamagedEffect->SetRelativeLocation(FVector(-50.0f, 0.0f, 100.0f));
	DamagedEffect->SetRelativeScale3D(FVector(0.6, 0.6, 0.6));


	//리스폰 이펙트

	RespawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_RESPAWN"));//이펙트 파티클 설정.
	RespawnEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_RESPAWN(TEXT("ParticleSystem'/Game/FX/P_Heal_Target.P_Heal_Target'"));
	if (P_RESPAWN.Succeeded())
	{
		RespawnEffect->SetTemplate(P_RESPAWN.Object);
		RespawnEffect->bAutoActivate = false;
	}

	RespawnEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	RespawnEffect->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.6f));


	//몬스터 리스폰 타임
	RespawnTime = 3.0f;

	

}

void AMyMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();



	MonsterAnim = Cast<UMonster_AnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == MonsterAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("MyAnim is null!"));
		return;
	}

	DeadEffect->OnSystemFinished.AddDynamic(this, &AMyMonster::OnEffectFinished);

	//HP가 Zero일때 관련 함수포인터 바인딩. 
	MyStat->OnHPIsZero.AddUObject(this, &AMyMonster::OnMonsterDead);

	////플레이어가 이 몬스터에게 락온중일때 델리게이트.
	//OnPlayerFocusOnThisDelegate.AddLambda([this]()->void {
	//	LockOnWidget->SetVisibility(true);
	//	});

	////플레이어의 락온에서 벗어났을 때 델리게이트.
	//OnLockOnRemoveThisDelegate.AddLambda([this]()->void {
	//	LockOnWidget->SetVisibility(false);
	//	});

	//AttackCheck노티파이 바인딩.
	MonsterAnim->OnAttackHitCheck.AddUObject(this, &AMyMonster::AttackCheck); //MyAnim에서 만든 델리게이트에 MyCharacter함수 바인딩.
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	//캐릭터 위젯 받아오기.
	auto MonsterWidget = Cast<UMyMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr == MonsterWidget) return;

	MonsterWidget->BindMonsterStat(MyStat);
	MonsterWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	OnMonsterMeetPlayerDelegate.AddLambda([this, MonsterWidget]()->void {
		MonsterWidget->SetVisibility(ESlateVisibility::Visible);
		});

	OnMonsterFartherAwayPlayerDelegate.AddLambda([this, MonsterWidget]()->void {
		MonsterWidget->SetVisibility(ESlateVisibility::Collapsed);
		});

	

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, 0.0f, 0.0f)); //메쉬 위치조정.


	GetCharacterMovement()->MaxWalkSpeed = MyStat->GetMoveSpeed(); //이동속도 조정

	//컨트롤러 받아오기.
	MyAIController = Cast<AMyAIController>(GetController());

	//원래 위치 저장.
	OriginLocation = GetActorLocation();
}

float AMyMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentAttacker = Cast<AMyPlayer>(DamageCauser);
	DamagedEffect->Activate(true); //피격 효과 재생
	if(MonsterAnim) MonsterAnim->PlayDamagedMontage();//피격 애니메이션 재생. 

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyMonster::SetDamage(float _Damage)
{
	MyStat->SetDamage(_Damage);
}

float AMyMonster::GetAttackPower()
{
	return MyStat->GetAttackPower();
}

void AMyMonster::Attack()
{
	if (MonsterAnim)
	{
		MonsterAnim->PlayAttackMontage(); //공격 몽타주 재생.
	}
}


void AMyMonster::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	//캐릭터 메쉬 숨기기.
	GetMesh()->SetVisibility(false);
	MonsterAnim->RestartAnimLogic();
	//타이머를 이용하여 일정시간이 지난 후 몬스터 리스폰.

	//각각의 타이머를 구분하는 데 사용하는 고유한 핸들.
	FTimerHandle MonsterRespawnTimer;
	//타이머 초기화.
	GetWorld()->GetTimerManager().SetTimer(MonsterRespawnTimer, FTimerDelegate::CreateUObject(this, &AMyMonster::OnMonsterRespawn), RespawnTime, false); // 반복 실행을 하고 싶다면 true.
}


void AMyMonster::OnMonsterRespawn()
{
	
	//원래 위치로 돌아가게 하기.
	SetActorLocation(OriginLocation);

	//AIController 다시 만들기.
	MyAIController->OnAIStart().Broadcast();
	
	//AnimInstance 다시 재생하도록 하기.
	

	//DeadEffect->Activate(true); // 죽음 이펙트 Activate.
	SetActorEnableCollision(true); // 콜리전 활성화.	
	MyStat->SetHPFull(); //피 다시 채워주기.
	//메쉬 보이게 하기.
	GetMesh()->SetVisibility(true);
	RespawnEffect->Activate(true);
}

void AMyMonster::OnMonsterDead()
{
	//GetController()->Destroy(); // AI 컨트롤러 Destroy.
	MyAIController->OnAIStop().Broadcast();
	CurrentAttacker->SetEXP(MyStat->GetEXPGive()); // 자신을 공격한 플레이어에게 경험치 주기.
	MonsterAnim->SetDeadAnim(); // 죽음 애니메이션 재생.
	DeadEffect->Activate(true); // 죽음 이펙트 Activate.
	SetActorEnableCollision(false); // 콜리전 비활성화.	

	if (LockOnComponent->GetIsLockOn()) //플레이어가 락온중인 상태였다면,
	{
		LockOnComponent->OnLockOnRemove().Broadcast(); // 몬스터의 락온 UI 지우고 isLockedOn변수 false.
		CurrentAttacker->RemoveLockOn(); // 플레이어의 락온도 해제해주기.
	}
}

