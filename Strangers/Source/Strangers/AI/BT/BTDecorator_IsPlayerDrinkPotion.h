#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayerDrinkPotion.generated.h"

UCLASS()
class STRANGERS_API UBTDecorator_IsPlayerDrinkPotion : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsPlayerDrinkPotion();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override; 
};
