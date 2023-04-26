// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Character/Player/MyPlayer.h"
#include "Components/WidgetComponent.h"
#include "AI/MyAIController.h"
#include "UI/MyMonsterWidget.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/Monster_AnimInstance.h"
#include "Character/Monster/MyMonsterStatComponent.h"

AMyMonster::AMyMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	

	HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET")); //HP바 위젯 컴포넌트 생성.
	HPBarWidget->SetupAttachment(GetMesh());

	MyStat = CreateDefaultSubobject<UMyMonsterStatComponent>(TEXT("MONSTERSTAT"));

	

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

	//HP가 Zero일때 관련 람다함수 선언, 바인딩. 
	MyStat->OnHPIsZero.AddLambda([this]() ->void {
		GetController()->Destroy();
		CurrentAttacker->SetEXP(MyStat->GetEXPGive());
		MonsterAnim->SetDeadAnim();	
		DeadEffect->Activate(true);
		SetActorEnableCollision(false);
		DeadEffect->OnSystemFinished.AddDynamic(this, &AMyMonster::OnEffectFinished);
		});

	//AttackCheck노티파이 바인딩.
	MonsterAnim->OnAttackHitCheck.AddUObject(this, &AMyMonster::AttackCheck); //MyAnim에서 만든 델리게이트에 MyCharacter함수 바인딩.
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	//캐릭터 위젯 받아오기.
	auto MonsterWidget = Cast<UMyMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr == MonsterWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterWidget is null!"));
		return;
	}
	MonsterWidget->BindMonsterStat(MyStat);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, 0.0f, 0.0f)); //메쉬 위치조정.

	GetCharacterMovement()->MaxWalkSpeed = MyStat->GetMoveSpeed(); //이동속도 조정
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
	Destroy();
}
