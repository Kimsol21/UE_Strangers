#include "AI/BT/BTService_RandomMaker.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/MyBossAIController.h"

UBTService_RandomMaker::UBTService_RandomMaker()
{
	NodeName = TEXT("RandomMaker");
	Interval = 0.5f; 
}

void UBTService_RandomMaker::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 

	uint8 ResultNum = 1;
	uint8 RandNum = FMath::RandRange(1, 100);
	uint8 TempNum = 100;

	for (uint8 i = 0; i < Percentages.Num(); i++)
	{
		TempNum = FMath::Clamp(TempNum - Percentages[i], 0, 100);
		if (TempNum < RandNum)
		{
			ResultNum = i;
			break;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyBossAIController::RandNumKey, ResultNum);

	return;
}
