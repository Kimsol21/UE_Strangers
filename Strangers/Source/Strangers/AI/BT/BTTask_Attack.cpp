#include "AI/BT/BTTask_Attack.h"
#include "AI/MyAIController.h"
#include "Character/Monster/MyMonster.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; 
	IsAttacking = false; 
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); 

	auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyMonster) return EBTNodeResult::Failed;

	MyMonster->Attack();
	IsAttacking = true;

	return EBTNodeResult::InProgress; 
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
