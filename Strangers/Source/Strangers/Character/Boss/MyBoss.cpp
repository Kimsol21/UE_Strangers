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


	//���� ������Ʈ ����� �ּ� �����ϱ�
	BossWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/Animations/Boss/SK_BossWeapon.SK_BossWeapon'"));
	if (SK_WEAPON.Succeeded())
	{
		BossWeaponMesh->SetSkeletalMesh(SK_WEAPON.Object);
	}
	BossWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BossWeaponMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 3.0f));
	



	CurrentHP = MaxHP;

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

	//��ƼŬ ���� �ҷ��� �� ����
	JumpAttackEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("JUMP_ATTACK_EFFECT"));//����Ʈ ��ƼŬ ����.
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

	// ���� AnimInstance ã�Ƽ� ��������� ����.
	BossAnim = Cast<UMyBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == BossAnim) return;

	BossAnim->OnBossAttackEnd().AddLambda([this]()->void {
		bIsAttackEnded = true; //���� ������ ���������� ����.
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
		JumpToTarget(1500.0f); // �÷��̾ ���� ����.
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
	
	
	//������ ���� Ŭ���� ĳ���Ϳ� ����.
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

	AttackPower = 10.0f;//���� �Ŀ�
	AttackRange = 300.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 200.0f;//���� ���� ���� ������(���� ����)

}

void AMyBoss::ExecuteNormalAttack2()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack2();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;

	AttackPower = 10.0f;//���� �Ŀ�
	AttackRange = 300.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 200.0f;//���� ���� ���� ������(���� ����)
}

void AMyBoss::ExecuteNormalAttack3()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack3();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false; // ������ �������� ������ false�� ����.

	AttackPower = 50.0f;//���� �Ŀ�
	AttackRange = 100.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 900.0f;//���� ���� ���� ������(���� ����)
	
}

void AMyBoss::ExecuteStab()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayStab();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;

	AttackPower = 20.0f;//���� �Ŀ�
	AttackRange = 600.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 100.0f;//���� ���� ���� ������(���� ����)
}

void AMyBoss::ExecuteKick()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayKick();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;

	AttackPower = 10.0f;//���� �Ŀ�
	AttackRange = 200.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 200.0f;//���� ���� ���� ������(���� ����)
}

void AMyBoss::ExecuteComboAttack()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayComboAttack();
	}

	//���ݰ��õ����� ����.
	bIsAttackEnded = false;

	AttackPower = 20.0f;//���� �Ŀ�
	AttackRange = 300.0f;//���� ���� ���� ������ ���� (���ݱ���)
	AttackRadius = 200.0f;//���� ���� ���� ������(���� ����)
}


void AMyBoss::JumpToTarget(float _JumpHeight)
{
	auto TargetToJump = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn()); //������ ��ǥ ĳ���� �޾ƿ���.
	auto JumpDestinationVec = TargetToJump->Camera->GetForwardVector(); //ĳ������ ī�޶� ��ġ���� �޾ƿ���.
	
	// ���� ���� ���� ���.
	JumpDestinationVec.X *= -1500.0f;
	JumpDestinationVec.Y *= -1500.0f;
	JumpDestinationVec.Z = _JumpHeight;

	LaunchCharacter(JumpDestinationVec, false, false); // ���� ����.
}

void AMyBoss::AttackCheck() //OnAttackCheck ��������Ʈ���� ȣ���� �Լ�.
{
	FHitResult HitResult; //�浹��� ���� ���� ���� ����ü.
	FName temp = NAME_None;
	FCollisionQueryParams Params(NAME_None, false, this);//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü.
	FVector StartLocation = GetActorLocation() + GetActorUpVector() * -130.0f +GetActorForwardVector()*100.0f; //�浹ó�� ������ġ.
	FVector TraceVec = GetActorForwardVector() * AttackRange; // ���� �̵��� ���� ��ġ����.
	/*
	* ù��° ���� (TraceTag) : Trace ������� ���� �߰� ���� �Ǵ� ���͸��� �����ϴ� �� ���Ǵ� �±�(��: Collision Analyzer)
	* �ι�° ���� (bTraceComplex) : ������ �浹�� ���� �����ؾ� �ϴ��� ����.
	* ����° ���� (IgnoreActor) : Trace�ϴ� ���� �����ؾ� �ϴ� ����.
	*/

	bool bResult = GetWorld()->SweepSingleByChannel( //Ʈ���̽� ü���� ����� ������ �浹���θ� ������ �Լ�.
		HitResult,
		StartLocation,//Ž��������ġ.
		StartLocation + TraceVec,//Ž�� ���� ��ġ.
		FQuat::Identity,//Ž���� ����� ������ ȸ��.
		ECollisionChannel::ECC_GameTraceChannel2,//���� �浹 ������ ����� Ʈ���̽� ä�� ����.
		FCollisionShape::MakeSphere(AttackRadius),//Ž���� ����� �⺻ ���� ����.(��ü,ĸ��,�ڽ� ��)
		Params
	);

//#if ENABLE_DRAW_DEBUG
//	FVector Center = StartLocation + TraceVec * 0.5f; //ĸ���� �߾ӿ� �ش��ϴ� ��.
//	float HalfHeight = AttackRange * 0.5f + AttackRadius; // ĸ�� ����.
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//ĸ���� Z���͸� ĳ���� �ü��������� ȸ��.
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule //DrawDebugHelpers���� �����ϴ� ĸ���׸��� �Լ�.
//	(
//		GetWorld(),//�׸�����
//		Center,//��ġ
//		HalfHeight,//ĸ������
//		AttackRadius,//������
//		CapsuleRot,//ĸ��ȸ��
//		DrawColor, //����
//		false,//���ӿ���
//		DebugLifeTime //���ӽð�
//	);
//#endif

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




