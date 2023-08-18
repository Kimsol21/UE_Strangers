#include "AI/BT/BTService_UpdateValue.h"
#include "BTService_UpdateValue.h"
#include "AI/MyBossAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Boss/MyBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MyBossAIController.h"

const float UBTService_UpdateValue::DetectRadiusClose(700.0f);
const float UBTService_UpdateValue::DetectRadiusFar(1600.0f);

UBTService_UpdateValue::UBTService_UpdateValue()
{
	NodeName = TEXT("UpdateValue"); 
	Interval = 0.5f; 
}

void UBTService_UpdateValue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto MyBoss = OwnerComp.GetAIOwner()->GetPawn();
	auto MyPlayer = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyBossAIController::TargetActorKey));
	if (nullptr == MyPlayer) return;

	float DistanceToTarget = MyBoss->GetDistanceTo(MyPlayer); 
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
