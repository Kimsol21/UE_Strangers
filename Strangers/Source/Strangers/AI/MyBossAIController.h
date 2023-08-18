#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyBossAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

DECLARE_MULTICAST_DELEGATE(FOnAIStopDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAIStartDelegate);

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

	FOnAIStopDelegate& OnAIStop() { return OnAIStopDelegate; };
	FOnAIStartDelegate& OnAIStart() { return OnAIStartDelegate; };

	static const FName TargetActorKey; 
	static const FName PhaseKey;
	static const FName IsDrinkPotion;
	static const FName DetactRange;
	static const FName RandNumKey;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

private:
	FOnAIStopDelegate OnAIStopDelegate;
	FOnAIStartDelegate OnAIStartDelegate;

	UPROPERTY()
	UBehaviorTree* BTAsset;

	UPROPERTY()
	UBlackboardData* BBAsset;
};
