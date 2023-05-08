// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossPatternAttack.generated.h"

/**
 * 
 */
 // ���� ���Ͽ� ���� Enum.
UENUM(BlueprintType)
enum class EBossPattern : uint8
{
	BOSSPATTERN_NORMAL_ATTACK1 UMETA(DisplayName = "NormalAttack1"),
	BOSSPATTERN_NORMAL_ATTACK2 UMETA(DisplayName = "NormalAttack2"),
	BOSSPATTERN_NORMAL_ATTACK3 UMETA(DisplayName = "NormalAttack3")
};

UCLASS()
class STRANGERS_API UBTTask_BossPatternAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossPatternAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attack Pattern")
	EBossPattern EPatternToExcute;//Enum _ ���� ��������.

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};