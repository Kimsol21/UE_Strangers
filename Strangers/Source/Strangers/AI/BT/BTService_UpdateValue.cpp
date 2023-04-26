// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_UpdateValue.h"
#include "BTService_UpdateValue.h"
#include "AI/MyBossAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Boss/MyBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MyBossAIController.h"

const float UBTService_UpdateValue::DetectRadiusClose(600.0f);//물체 감지 먼 반경 10미터로 설정.
const float UBTService_UpdateValue::DetectRadiusFar(1600.0f);// 물체 감지 가까운 반경 6미터로 설정.

UBTService_UpdateValue::UBTService_UpdateValue()
{
	NodeName = TEXT("UpdateValue"); //노드 이름 설정.
	Interval = 0.5f; // 실행 빈도 설정. 	
}


void UBTService_UpdateValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // 부모함수 먼저 실행.

	//static AMyBoss* MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // 보스 받아오기.
	//if (nullptr == MyBoss) return;

	//static AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetWorld()->GetFirstLocalPlayerFromController()); // 플레이어 받아와 저장.
	//if (nullptr == MyPlayer) return;

	auto MyBoss = OwnerComp.GetAIOwner()->GetPawn();
	auto MyPlayer = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyBossAIController::TargetActorKey));
	if (nullptr == MyPlayer) return;

	//블랙보드 값 갱신.
	float DistanceToTarget = MyBoss->GetDistanceTo(MyPlayer); //거리에 따른 Enum값 대입.
	if (DetectRadiusClose >DistanceToTarget)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyBossAIController::DetactRange, uint8(EDetactRange::RANGE_SMALL));
	}
	else if ( DetectRadiusFar>DistanceToTarget )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyBossAIController::DetactRange, uint8(EDetactRange::RANGE_MIDDLE));
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMyBossAIController::DetactRange, uint8(EDetactRange::RANGE_BIG));
	}

	
}
