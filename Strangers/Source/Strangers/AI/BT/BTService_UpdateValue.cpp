// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_UpdateValue.h"
#include "BTService_UpdateValue.h"
#include "AI/MyBossAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Boss/MyBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MyBossAIController.h"

const float UBTService_UpdateValue::DetectRadiusClose(600.0f);//��ü ���� �� �ݰ� 10���ͷ� ����.
const float UBTService_UpdateValue::DetectRadiusFar(1600.0f);// ��ü ���� ����� �ݰ� 6���ͷ� ����.

UBTService_UpdateValue::UBTService_UpdateValue()
{
	NodeName = TEXT("UpdateValue"); //��� �̸� ����.
	Interval = 0.5f; // ���� �� ����. 	
}


void UBTService_UpdateValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // �θ��Լ� ���� ����.

	//static AMyBoss* MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // ���� �޾ƿ���.
	//if (nullptr == MyBoss) return;

	//static AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetWorld()->GetFirstLocalPlayerFromController()); // �÷��̾� �޾ƿ� ����.
	//if (nullptr == MyPlayer) return;

	auto MyBoss = OwnerComp.GetAIOwner()->GetPawn();
	auto MyPlayer = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyBossAIController::TargetActorKey));
	if (nullptr == MyPlayer) return;

	//������ �� ����.
	float DistanceToTarget = MyBoss->GetDistanceTo(MyPlayer); //�Ÿ��� ���� Enum�� ����.
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
