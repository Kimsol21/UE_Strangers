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
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Boss_BloodHand.BB_Boss_BloodHand'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Boss_BloodHand.BT_Boss_BloodHand'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	OnAIStartDelegate.AddLambda([this]()->void {
		GetBrainComponent()->ResumeLogic("Boss AI Begin");
		});

	OnAIStopDelegate.AddLambda([this]()->void {
		GetBrainComponent()->PauseLogic("Boss AI Stop");
		});
}

void AMyBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsInt(PhaseKey, 1); 
		Blackboard->SetValueAsEnum(DetactRange, uint8(EDetactRange::RANGE_BIG)); 

		AMyBoss* PossessedPawn = Cast<AMyBoss>(GetPawn());
		if (nullptr == PossessedPawn) return;

		PossessedPawn->OnPhaseChanged.AddLambda([this, PossessedPawn]()->void {
			Blackboard->SetValueAsInt(PhaseKey, PossessedPawn->GetPhase());
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

	Blackboard->SetValueAsObject(TargetActorKey, GetWorld()->GetFirstPlayerController()->GetPawn());
	Blackboard->SetValueAsInt(RandNumKey, 1);
}
