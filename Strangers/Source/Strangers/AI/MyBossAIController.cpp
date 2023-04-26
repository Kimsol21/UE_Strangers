// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MyBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Character/Boss/MyBoss.h"

const FName AMyBossAIController::TargetActorKey(TEXT("TargetActor"));
const FName AMyBossAIController::PhaseKey(TEXT("Phase"));
const FName AMyBossAIController::IsDrinkPotion(TEXT("IsDrinkPotion"));
const FName AMyBossAIController::DetactRange(TEXT("DetactRange"));
const FName AMyBossAIController::RandNumKey(TEXT("RandNum"));


AMyBossAIController::AMyBossAIController() 
{
	//블랙보드 어셋 불러오기
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Boss_BloodHand.BB_Boss_BloodHand'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	//비헤이비어트리 어셋 불러오기
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Boss_BloodHand.BT_Boss_BloodHand'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

//Pawn에 빙의되었을때 호출되는 함수. 
void AMyBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		//BT 돌리기 전에 BB 값 초기화.
		Blackboard->SetValueAsInt(PhaseKey, 1); //페이즈 1로 초기화.
		Blackboard->SetValueAsEnum(DetactRange, uint8(EDetactRange::RANGE_BIG)); //거리는 멀음으로 초기화.
		//Blackboard->SetValueAsObject(TargetActorKey, GetWorld()->GetFirstLocalPlayerFromController()); //플레이어 컨트롤러를 타겟 액터로 초기화.

		AMyBoss* PossessedPawn = Cast<AMyBoss>(GetPawn());
		if (nullptr == PossessedPawn) return;
		PossessedPawn->OnPhaseChanged.AddLambda([this, PossessedPawn]()->void {
			UE_LOG(LogTemp, Error, TEXT("Phase Change!!!!!!!!!!!!!!!"));
			Blackboard->SetValueAsInt(PhaseKey, PossessedPawn->GetPhase()); //페이즈가 바뀌면 블랙보드값 바꿔줄 델리게이트.
			});


		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn't run behavior tree!!"));
			
		}
	}
}

void AMyBossAIController::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsObject(TargetActorKey, GetWorld()->GetFirstPlayerController()->GetPawn()); //플레이어 컨트롤러를 타겟 액터로 초기화.
	Blackboard->SetValueAsInt(RandNumKey, 1);
}
