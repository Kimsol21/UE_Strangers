// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyBossAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

/**
 * 
 */
 // �÷��̾ ��� �Ÿ������� �����ִ����� ���� Enum.
UENUM(BlueprintType)
enum class EDetactRange : uint8
{
	RANGE_SMALL UMETA(DisplayName = "SmallRange"),
	RANGE_MIDDLE UMETA(DisplayName = "MiddleRange"),
	RANGE_BIG UMETA(DisplayName = "BigRange")
};

UCLASS()
class STRANGERS_API AMyBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyBossAIController();

	//������ Ű������ �̸��� ������ ����. ���� ������ ���� �������̹Ƿ� static const�� ����� �����Ѵ�. 
	static const FName TargetActorKey; 
	static const FName PhaseKey;
	static const FName IsDrinkPotion;
	static const FName DetactRange;
	static const FName RandNumKey;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	UBehaviorTree* BTAsset;

	UPROPERTY()
	UBlackboardData* BBAsset;
};
