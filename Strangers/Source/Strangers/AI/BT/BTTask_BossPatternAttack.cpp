// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_BossPatternAttack.h"
#include "Character/Boss/MyBoss.h"
#include "AI/MyBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossPatternAttack::UBTTask_BossPatternAttack()
{
	bNotifyTick = true; // 이 값이 true이면 TickTask() 함수가 호출됩니다.
	EPatternToExcute = EBossPattern::BOSSPATTERN_NORMAL_ATTACK1; // 보스 패턴 디폴트값.
}

//테스크 실행 함수.
EBTNodeResult::Type UBTTask_BossPatternAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //부모의 함수 먼저 실행 후 결과값 저장.

	OwnerPawn = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // BT 소유주 불러오기.
	if (nullptr == OwnerPawn) return EBTNodeResult::Failed; // 예외처리

	//if (MyBoss->BossState == EBossState::ATTACKING_START) return EBTNodeResult::Succeeded;

	switch (EPatternToExcute)
	{
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK1:
		OwnerPawn->ExecuteNormalAttack1();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK2:
		OwnerPawn->ExecuteNormalAttack2();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK3:
		OwnerPawn->ExecuteNormalAttack3();
		break;
	case EBossPattern::BOSSPATTERN_STAB:
		OwnerPawn->ExecuteStab();
		break;
	case EBossPattern::BOSSPATTERN_KICK:
		OwnerPawn->ExecuteKick();
		break;
	case EBossPattern::BOSSPATTERN_COMBO_ATTACK:
		OwnerPawn->ExecuteComboAttack();
		break;
	default:
		break;
	}
	OwnerPawn->BossState = EBossState::ATTACKING_START;
	

	return EBTNodeResult::InProgress; // 테스크가 진행중임을 return.
}

void UBTTask_BossPatternAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	if (OwnerPawn->BossState == EBossState::ATTACKING_END)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크 종료함수. 테스크 성공 반환. 
		OwnerPawn->BossState = EBossState::NORMALL;		
		return;
	}
	
	

	// 보스 상태가 공격중이었고, Attack몽타주가 끝났으면,
	

	return;
}