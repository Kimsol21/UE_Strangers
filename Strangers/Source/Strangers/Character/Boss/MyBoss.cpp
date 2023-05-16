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



const float AMyBoss::MaxHP(100.0f);

// Sets default values
AMyBoss::AMyBoss() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CurrentHP = MaxHP;
	AttackPower = 100.0f;

	//���̷�Ż �޽� ���� �ҷ�����.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_BOSS(TEXT("SkeletalMesh'/Game/Animations/Boss/SM_Boss.SM_Boss'"));
	if (SM_BOSS.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_BOSS.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint); //�ִϸ��̼� ��� �ִԺ������Ʈ ���� ����. 
	GetMesh()->SetRelativeLocation(FVector(0.0f, -10.0f, -150.0f)); //�޽� ��ġ�� ���߱�.
	GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); //�޽� �����ϰ� �����ϱ�.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -147.0f), FRotator(0.0, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.0f); //ĸ�� ũ�� ����.
	GetCapsuleComponent()->SetCapsuleRadius(60.0f); // ĸ�� �ʺ� ����.
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); //ĸ�� �����ϰ� ����.


	//AnimBP Ŭ�������� ��������.
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Boss(TEXT("AnimBlueprint'/Game/Animations/Boss/AnimBP_Boss.AnimBP_Boss_C'"));
	if (ABP_Boss.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Boss.Class);
	}

	//AI ���� �ɼ� ���� : ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� MyBoss ��ü���� MyBossAIController ����.
	AIControllerClass = AMyBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster")); // ���� ���� �ݸ��� ������ ���.


	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LOCKON")); //���� ������Ʈ ����.


	//��ƼŬ ���� �ҷ��� �� ����
	DeadEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));//����Ʈ ��ƼŬ ����.
	DeadEffect->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DEADEFFECT(TEXT("ParticleSystem'/Game/FX/Effect_BossDead1.Effect_BossDead1'"));
	if (P_DEADEFFECT.Succeeded())
	{
		DeadEffect->SetTemplate(P_DEADEFFECT.Object);
		DeadEffect->bAutoActivate = false;
	}
	DeadEffect->SetRelativeScale3D(FVector(2.0, 2.0, 2.0));
	DeadEffect->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

void AMyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ���� AnimInstance ã�Ƽ� ��������� ����.
	BossAnim = Cast<UMyBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == BossAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("Boss AnimInstance is null"));
		return;
	}

	BossAnim->OnBossAttackEnd().AddLambda([this]()->void {
		bIsAttackEnded = true;
	});

	BossAnim->OnChangeFlyingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		});

	BossAnim->OnChangeWalkingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		});

	BossAnim->OnExcuteBossJump().AddLambda([this]()->void {
		JumpToTarget(1500.0f); // �÷��̾ ���� ����.
		});

	DeadEffect->OnSystemFinished.AddDynamic(this, &AMyBoss::OnEffectFinished);
}

// Called when the game starts or when spawned
void AMyBoss::BeginPlay()
{
	Super::BeginPlay();
	
	//AIController �޾ƿ���.
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
		//������ �ó׸�ƽ �������� �� ��������Ʈ.
		MyPlayerController->OnLevelSequenceStart().AddLambda([this]()->void {
			SetActorHiddenInGame(true);
			});
		
		//������ �������� �� ��������Ʈ/
		MyPlayerController->OnBossFightStart().AddLambda([this](AMyBoss* _MyBoss)->void {
			SetActorHiddenInGame(false);
			BossAIController->OnAIStart().Broadcast();
			});
	}
	//BossWidget->BindWidgetToBoss(this);

	SetActorHiddenInGame(true);
	BossAIController->OnAIStop().Broadcast();
	
}

// Called every frame
void AMyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

float AMyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.0f, MaxHP); //�ֱ� HP���� ������ ���� ��ŭ ����.

	if (0 == CurrentHP) //ü���� 0���� �������� �̺�Ʈ �߻���Ű��.
	{
		OnBossHPIsZeroEvent.Broadcast();
		DeadEffect->Activate(true);
	}
	else if (2 * CurrentHP < MaxHP && 1==Phase)//ü���� 50�ۼ�Ʈ ���Ϸ� ��������,
	{
		Phase = 2;//2������� ����.
		OnPhaseChanged.Broadcast();
	}

	SetHP(CurrentHP);
	return 0.0f;
}

void AMyBoss::SetHP(const float& _NewHP) 
{ 
	CurrentHP = _NewHP; 
	BossHPChangedEvent.Broadcast(CurrentHP / MaxHP); // �̺�Ʈ�� ���� HP ���� ����. 

}

void AMyBoss::ExecuteNormalAttack1()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack1();
	}	

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.
	AttackPower = 10.0f;//���� ������
}

void AMyBoss::ExecuteNormalAttack2()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack2();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.
	AttackPower = 10.0f;//���� ������
}

void AMyBoss::ExecuteNormalAttack3()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack3();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //���� ������ ����.
	AttackRadius = 50.0f;//�� ������.
	AttackPower = 10.0f;//���� ������
}


void AMyBoss::JumpToTarget(float _JumpPower)
{
	auto TargetToJump = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()); //������ ��ǥ ĳ���� �޾ƿ���.
	auto JumpDestinationVec = TargetToJump->Camera->GetForwardVector(); //ĳ������ ī�޶� ��ġ���� �޾ƿ���.
	
	// ���� ���� ���� ���.
	JumpDestinationVec.X *= -500.0f;
	JumpDestinationVec.Y *= -500.0f;
	JumpDestinationVec.Z = _JumpPower;

	LaunchCharacter(JumpDestinationVec, true, false); // ���� ����.
}

void AMyBoss::AttackCheck() //OnAttackCheck ��������Ʈ���� ȣ���� �Լ�.
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
			HitResult.Actor->TakeDamage(AttackPower, DamageEvent, GetController(), this);//AActor���� �����ϴ� �Լ�. (������ ������ ����, ����������, ������, �������ظ����� Actor)
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




