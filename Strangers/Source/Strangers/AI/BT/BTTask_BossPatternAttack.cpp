// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_BossPatternAttack.h"
#include "Character/Boss/MyBoss.h"
#include "AI/MyBossAIController.h"

UBTTask_BossPatternAttack::UBTTask_BossPatternAttack()
{
	bNotifyTick = true; // 이 값이 true이면 TickTask() 함수가 호출됩니다.
	EPatternToExcute = EBossPattern::BOSSPATTERN_NORMAL_ATTACK1; // 보스 패턴 디폴트값.
}

//테스크 실행 함수.
EBTNodeResult::Type UBTTask_BossPatternAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //부모의 함수 먼저 실행 후 결과값 저장.

	auto MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // BT 소유주 불러오기.
	if (nullptr == MyBoss) return EBTNodeResult::Failed; // 예외처리

	switch (EPatternToExcute)
	{
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK1:
		MyBoss->ExecuteNormalAttack1();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK2:
		MyBoss->ExecuteNormalAttack2();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK3:
		MyBoss->ExecuteNormalAttack3();
		break;
	default:
		MyBoss->ExecuteNormalAttack1();
		break;
	}

	return EBTNodeResult::InProgress; // 테스크가 진행중임을 return.
}

void UBTTask_BossPatternAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	static AMyBoss* MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // BT 소유주 불러오기.
	if (nullptr == MyBoss) return; // 예외처리

	if (MyBoss->GetIsAttackEnded()) //Attack몽타주가 끝났으면,
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크 종료함수. 테스크 성공 반환. 
		UE_LOG(LogTemp, Error, TEXT("Task Succeeded Return"));
		return;
	}
}