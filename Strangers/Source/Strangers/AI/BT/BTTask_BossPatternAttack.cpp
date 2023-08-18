#include "AI/BT/BTTask_BossPatternAttack.h"
#include "Character/Boss/MyBoss.h"
#include "AI/MyBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossPatternAttack::UBTTask_BossPatternAttack()
{
	bNotifyTick = true; 
	EPatternToExcute = EBossPattern::BOSSPATTERN_NORMAL_ATTACK1; 
}

EBTNodeResult::Type UBTTask_BossPatternAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); 

	OwnerPawn = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); 
	if (nullptr == OwnerPawn) return EBTNodeResult::Failed;

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
	
	return EBTNodeResult::InProgress;
}

void UBTTask_BossPatternAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerPawn->BossState == EBossState::ATTACKING_END)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		OwnerPawn->BossState = EBossState::NORMALL;		
	}
	return;
}