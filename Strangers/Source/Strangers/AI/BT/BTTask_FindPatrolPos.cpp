// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPatrolPos.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}


//BehaviorTree는 Task를 실행할 때 Task Class의 ExecuteTask라는 멤버 함수를 실행한다. 이 함수는 Aborted, Failed, Succeeded, InProgress 이렇게 네가지값을 반환한다. 
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);//블랙보드의 HomPosKey값을 Orgin에 저장.
	FNavLocation NextPatrol;

	bool myTest = NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol);
	if (myTest) //인자 : 원래위치, 반경, 저장할위치
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
