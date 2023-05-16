// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMyAIController::HomePosKey(TEXT("HomePos"));
const FName AMyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMyAIController::TargetKey(TEXT("Target"));

AMyAIController::AMyAIController()
{
	//블랙보드 어셋 불러오기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_MyCharacter.BB_MyCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	//비헤이비어트리 어셋 불러오기
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_MyCharacter.BT_MyCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}


	//인공지능 시작하기.
	OnAIStartDelegate.AddLambda([this]()->void {
		GetBrainComponent()->ResumeLogic("Monster is Respawned");
		});

	//인공지능 멈추기.
	OnAIStopDelegate.AddLambda([this]()->void {
		GetBrainComponent()->PauseLogic("Monster is Dead");
		});
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp,Error, TEXT("AIController couldn't run behavior tree!!"));
		}
		//MoveToActor(InPawn);
		
	}

}
