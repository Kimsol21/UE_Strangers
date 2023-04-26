// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomMaker.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UBTService_RandomMaker : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_RandomMaker();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Random")
	TArray<uint8> Percentages;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

};
