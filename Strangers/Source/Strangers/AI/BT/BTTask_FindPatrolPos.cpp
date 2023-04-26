// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPatrolPos.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos"); //��� �⺻ �̸� ����.
}


//BehaviorTree�� Task�� ������ �� Task Class�� ExecuteTask��� ��� �Լ��� �����Ѵ�. �� �Լ��� Aborted, Failed, Succeeded, InProgress �̷��� �װ������� ��ȯ�Ѵ�. 
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);//�θ� �׽�ũ ���� �����ϰ� ������� ����.

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // BT������ �ҷ��� ������ ����.
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // ����ó��.

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); // ������ ���̰��̼� �ý��� �ҷ��� ����.
	if (nullptr == NavSystem) return EBTNodeResult::Failed; // ����ó��.

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);//�������� HomPosKey���� ������ ����.
	FNavLocation NextPatrol; // ���� ������ ��ġ ���� ����.

	bool myTest = NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol); // �ʱ� ��ġ�κ��� �ݰ� 5���� ���� ������ ��ġ�� ������ ��ġ�� �������ش�.
	if (myTest) //���� �۾��� �����ߴٸ�,
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, NextPatrol.Location);//�������� ������ ��ġ ���� �������ش�.
		return EBTNodeResult::Succeeded; // �׽�ũ����� ������ ��ȯ�Ѵ�.
	}
	return EBTNodeResult::Failed; // ���и� ��ȯ�Ѵ�. 
}
