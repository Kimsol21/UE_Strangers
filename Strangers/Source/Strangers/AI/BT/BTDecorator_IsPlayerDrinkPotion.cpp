#include "AI/BT/BTDecorator_IsPlayerDrinkPotion.h"
#include "AI/MyBossAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsPlayerDrinkPotion::UBTDecorator_IsPlayerDrinkPotion()
{
	NodeName = TEXT("InProgress");
}

bool UBTDecorator_IsPlayerDrinkPotion::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMyPlayer* Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyBossAIController::TargetActorKey));
	if (nullptr == Target) return false;

	return Target->GetIsDrinkPotion();
}
