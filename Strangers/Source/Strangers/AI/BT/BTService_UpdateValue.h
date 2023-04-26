// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateValue.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UBTService_UpdateValue : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateValue();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	static const float DetectRadiusFar; 
	static const float DetectRadiusClose;
};
