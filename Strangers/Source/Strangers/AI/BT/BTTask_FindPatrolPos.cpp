#include "AI/BT/BTTask_FindPatrolPos.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos"); 
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); 
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; 

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); 
	if (nullptr == NavSystem) return EBTNodeResult::Failed; 

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);
	FNavLocation NextPatrol; 

	bool myTest = NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol); // �ʱ� ��ġ�κ��� �ݰ� 5���� ���� ������ ��ġ�� ������ ��ġ�� ����
	if (myTest)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed; 
}
