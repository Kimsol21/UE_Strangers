// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_RandomMaker.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MyBossAIController.h"


UBTService_RandomMaker::UBTService_RandomMaker()
{
	NodeName = TEXT("RandomMaker"); //노드 이름 설정.
	Interval = 1.0f; // 실행 빈도 설정.
}

void UBTService_RandomMaker::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); //부모의 함수 먼저 실행.

	uint8 ResultNum = 1;

	uint8 RandNum = FMath::RandRange(1, 100);
	uint8 TempNum = 100;

	
	for (uint8 i = 0; i < Percentages.Num(); i++)
	{
		TempNum = FMath::Clamp(TempNum - Percentages[i], 0, 100);
		if (TempNum < RandNum)
		{
			ResultNum = i + 1;
			break;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyBossAIController::RandNumKey, ResultNum);
	//UE_LOG(LogTemp, Error, TEXT("SetRandomNum to Blackboard : %d"), ResultNum); 랜덤넘버 출력 확인.

	return;
}
