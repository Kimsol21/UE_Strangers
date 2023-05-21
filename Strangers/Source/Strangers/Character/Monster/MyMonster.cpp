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

	HPBarWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("HPBARWIDGET")); //HP�� ���� ������Ʈ ����.
	HPBarWidget->SetupAttachment(GetMesh());

	MyStat = CreateDefaultSubobject<UMyMonsterStatComponent>(TEXT("MONSTERSTAT"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LOCKON")); //���� ������Ʈ ����.
	

	//���� �ҷ�����.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_CORPSE(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Corpse_Basic/Corpse_Basic.Corpse_Basic'"));
	if (SM_CORPSE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_CORPSE.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KWANG(TEXT("AnimBlueprint'/Game/Animations/Monster/Monster_AnimBlueprint.Monster_AnimBlueprint_C'")); //Ŭ�������� ��������.
	if (ABP_KWANG.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_KWANG.Class);
	}

	//���� ������Ʈ ��ġ ���� & ������ �������Ʈ �ּ��� Ŭ���� ������ ���� ������Ʈ�� widgetClass�� ���.
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//�׻� �÷��̾� �ٶ󺸱�.

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	////���� Circle UI
	//LockOnWidget = CreateDefaultSubobject <UWidgetComponent>(TEXT("LockOnWidget")); //HP�� ���� ������Ʈ ����.
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
	


	//AI ���� �ɼ� ���� : ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� MyMonster��ü���� MyAIController���� ����.
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));//���� ���� �ݸ��� ������ ���.

	//��ƼŬ ���� �ҷ��� �� ����
	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));//����Ʈ ��ƼŬ ����.
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
	DamagedEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT2"));//����Ʈ ��ƼŬ ����.
	DamagedEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DAMAGED(TEXT("ParticleSystem'/Game/FX/Particle_Damaged.Particle_Damaged'"));
	if (P_DAMAGED.Succeeded())
	{
		DamagedEffect->SetTemplate(P_DAMAGED.Object);
		DamagedEffect->bAutoActivate = false;
	}

	DamagedEffect->SetRelativeLocation(FVector(-50.0f, 0.0f, 100.0f));
	DamagedEffect->SetRelativeScale3D(FVector(0.6, 0.6, 0.6));


	//������ ����Ʈ

	RespawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_RESPAWN"));//����Ʈ ��ƼŬ ����.
	RespawnEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_RESPAWN(TEXT("ParticleSystem'/Game/FX/P_Heal_Target.P_Heal_Target'"));
	if (P_RESPAWN.Succeeded())
	{
		RespawnEffect->SetTemplate(P_RESPAWN.Object);
		RespawnEffect->bAutoActivate = false;
	}

	RespawnEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	RespawnEffect->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.6f));


	//���� ������ Ÿ��
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

	//HP�� Zero�϶� ���� �Լ������� ���ε�. 
	MyStat->OnHPIsZero.AddUObject(this, &AMyMonster::OnMonsterDead);

	////�÷��̾ �� ���Ϳ��� �������϶� ��������Ʈ.
	//OnPlayerFocusOnThisDelegate.AddLambda([this]()->void {
	//	LockOnWidget->SetVisibility(true);
	//	});

	////�÷��̾��� ���¿��� ����� �� ��������Ʈ.
	//OnLockOnRemoveThisDelegate.AddLambda([this]()->void {
	//	LockOnWidget->SetVisibility(false);
	//	});

	//AttackCheck��Ƽ���� ���ε�.
	MonsterAnim->OnAttackHitCheck.AddUObject(this, &AMyMonster::AttackCheck); //MyAnim���� ���� ��������Ʈ�� MyCharacter�Լ� ���ε�.
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	//ĳ���� ���� �޾ƿ���.
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

	

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0, 0.0f, 0.0f)); //�޽� ��ġ����.


	GetCharacterMovement()->MaxWalkSpeed = MyStat->GetMoveSpeed(); //�̵��ӵ� ����

	//��Ʈ�ѷ� �޾ƿ���.
	MyAIController = Cast<AMyAIController>(GetController());

	//���� ��ġ ����.
	OriginLocation = GetActorLocation();
}

float AMyMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentAttacker = Cast<AMyPlayer>(DamageCauser);
	DamagedEffect->Activate(true); //�ǰ� ȿ�� ���
	if(MonsterAnim) MonsterAnim->PlayDamagedMontage();//�ǰ� �ִϸ��̼� ���. 

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
		MonsterAnim->PlayAttackMontage(); //���� ��Ÿ�� ���.
	}
}


void AMyMonster::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	//ĳ���� �޽� �����.
	GetMesh()->SetVisibility(false);
	MonsterAnim->RestartAnimLogic();
	//Ÿ�̸Ӹ� �̿��Ͽ� �����ð��� ���� �� ���� ������.

	//������ Ÿ�̸Ӹ� �����ϴ� �� ����ϴ� ������ �ڵ�.
	FTimerHandle MonsterRespawnTimer;
	//Ÿ�̸� �ʱ�ȭ.
	GetWorld()->GetTimerManager().SetTimer(MonsterRespawnTimer, FTimerDelegate::CreateUObject(this, &AMyMonster::OnMonsterRespawn), RespawnTime, false); // �ݺ� ������ �ϰ� �ʹٸ� true.
}


void AMyMonster::OnMonsterRespawn()
{
	
	//���� ��ġ�� ���ư��� �ϱ�.
	SetActorLocation(OriginLocation);

	//AIController �ٽ� �����.
	MyAIController->OnAIStart().Broadcast();
	
	//AnimInstance �ٽ� ����ϵ��� �ϱ�.
	

	//DeadEffect->Activate(true); // ���� ����Ʈ Activate.
	SetActorEnableCollision(true); // �ݸ��� Ȱ��ȭ.	
	MyStat->SetHPFull(); //�� �ٽ� ä���ֱ�.
	//�޽� ���̰� �ϱ�.
	GetMesh()->SetVisibility(true);
	RespawnEffect->Activate(true);
}

void AMyMonster::OnMonsterDead()
{
	//GetController()->Destroy(); // AI ��Ʈ�ѷ� Destroy.
	MyAIController->OnAIStop().Broadcast();
	CurrentAttacker->SetEXP(MyStat->GetEXPGive()); // �ڽ��� ������ �÷��̾�� ����ġ �ֱ�.
	MonsterAnim->SetDeadAnim(); // ���� �ִϸ��̼� ���.
	DeadEffect->Activate(true); // ���� ����Ʈ Activate.
	SetActorEnableCollision(false); // �ݸ��� ��Ȱ��ȭ.	

	if (LockOnComponent->GetIsLockOn()) //�÷��̾ �������� ���¿��ٸ�,
	{
		LockOnComponent->OnLockOnRemove().Broadcast(); // ������ ���� UI ����� isLockedOn���� false.
		CurrentAttacker->RemoveLockOn(); // �÷��̾��� ���µ� �������ֱ�.
	}
}

