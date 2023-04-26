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


}

void AMyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 보스 AnimInstance 찾아서 멤버변수에 저장.
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

	if (2 * (CurrentHP - FinalDamage) < MaxHP)//체력이 50퍼센트 이하로 떨어지면,
	{
		Phase = 2;//2페이즈로 진입.
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
	

	//공격관련데이터 설정.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.
	AttackPower = 10.0f;//공격 데미지
}

void AMyBoss::ExecuteNormalAttack2()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack2();
	}
	

	//공격관련데이터 설정.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.
	AttackPower = 10.0f;//공격 데미지
}

void AMyBoss::ExecuteNormalAttack3()
{
	if (BossAnim)
	{
		BossAnim->MontagePlayNormalAttack3();
	}
	

	//공격관련데이터 설정.
	bIsAttackEnded = false;
	AttackRange = 200.0f; //구가 지나갈 길이.
	AttackRadius = 50.0f;//구 반지름.
	AttackPower = 10.0f;//공격 데미지
}

void AMyBoss::AttackCheck() //OnAttackCheck 델리게이트에서 호출할 함수.
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
			HitResult.Actor->TakeDamage(AttackPower, DamageEvent, GetController(), this);//AActor에서 제공하는 함수. (전달할 데미지 세기, 데미지종류, 가해자, 직접피해를입힌 Actor)
		}
	}
}

