// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Boss/MyBoss.h"
#include "Components/CapsuleComponent.h"
#include "AI/MyBossAIController.h"
#include "Animation/MyBossAnimInstance.h"
#include "DrawDebugHelpers.h" 
#include "GameFramework/CharacterMovementComponent.h"

const float AMyBoss::MaxHP(1000.0f);

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

	BossAnim->OnBossAttackEnd.AddLambda([this]()->void {
		bIsAttackEnded = true;
		UE_LOG(LogTemp, Error, TEXT("OnBossAttackEndDelegate@@@@@@@@@@@@@@@"));
	});

	BossAnim->OnChangeFlyingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		});

	BossAnim->OnChangeWalkingMode.AddLambda([this]()->void {
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		});
}

// Called when the game starts or when spawned
void AMyBoss::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AMyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//CurrentHP

	if (2 * (CurrentHP - FinalDamage) < MaxHP)//ü���� 50�ۼ�Ʈ ���Ϸ� ��������,
	{
		Phase = 2;//2������� ����.
		OnPhaseChanged.Broadcast();
	}

	SetHP(FMath::Clamp(CurrentHP - FinalDamage, 0.0f, MaxHP));
	return 0.0f;
}

void AMyBoss::SetHP(const float& _NewHP) 
{ 
	CurrentHP = _NewHP; 
	OnBossHPChanged.Broadcast();

	if (0 == CurrentHP)
	{
		OnBossHPIsZero.Broadcast();
	}
}

//void AMyBoss::SetFlyingMode()
//{
//	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
//}
//
//void AMyBoss::SetWalkingMode()
//{
//	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
//}

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

