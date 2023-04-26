// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayerDrinkPotion.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UBTDecorator_IsPlayerDrinkPotion : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsPlayerDrinkPotion();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override; //원하는 조건이 달성됬는지 파악
};
