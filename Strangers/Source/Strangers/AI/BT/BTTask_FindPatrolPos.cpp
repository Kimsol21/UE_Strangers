// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPatrolPos.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos"); //노드 기본 이름 지정.
}


//BehaviorTree는 Task를 실행할 때 Task Class의 ExecuteTask라는 멤버 함수를 실행한다. 이 함수는 Aborted, Failed, Succeeded, InProgress 이렇게 네가지값을 반환한다. 
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);//부모 테스크 먼저 시행하고 결과값에 저장.

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // BT소유주 불러와 변수에 저장.
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed; // 예외처리.

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); // 월드의 네이게이션 시스템 불러와 저장.
	if (nullptr == NavSystem) return EBTNodeResult::Failed; // 예외처리.

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);//블랙보드의 HomPosKey값을 변수에 저장.
	FNavLocation NextPatrol; // 다음 순찰할 위치 변수 선언.

	bool myTest = NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol); // 초기 위치로부터 반경 5미터 내에 랜덤한 위치를 순찰할 위치로 저장해준다.
	if (myTest) //위의 작업이 성공했다면,
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, NextPatrol.Location);//블랙보드의 순찰할 위치 값에 저장해준다.
		return EBTNodeResult::Succeeded; // 테스크노드의 성공을 반환한다.
	}
	return EBTNodeResult::Failed; // 실패를 반환한다. 
}
