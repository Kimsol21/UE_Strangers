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
 // 플레이어가 어느 거리범위에 속해있는지에 관한 Enum.
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

	//블랙보드 키값들의 이름을 저장할 변수. 절대 변하지 않을 데이터이므로 static const를 사용해 선언한다. 
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
