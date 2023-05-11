// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAIStopDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAIStartDelegate);


UCLASS()
class STRANGERS_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;


	static const FName HomePosKey; //앞으로 절대 변하지 않을 데이터에 static const를 사용해 선언한다.
	static const FName PatrolPosKey;
	static const FName TargetKey;


	FOnAIStopDelegate& OnAIStop() { return OnAIStopDelegate; };
	FOnAIStartDelegate& OnAIStart() { return OnAIStartDelegate; };
private:
	FOnAIStopDelegate OnAIStopDelegate;
	FOnAIStartDelegate OnAIStartDelegate;

	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;


};
