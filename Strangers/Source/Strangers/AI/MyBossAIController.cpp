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
	//������ ��� �ҷ�����
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Boss_BloodHand.BB_Boss_BloodHand'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	//�����̺��Ʈ�� ��� �ҷ�����
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Boss_BloodHand.BT_Boss_BloodHand'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	//�ΰ����� �����ϱ�.
	OnAIStartDelegate.AddLambda([this]()->void {
		GetBrainComponent()->ResumeLogic("Boss AI Begin");
		UE_LOG(LogTemp, Warning, TEXT("Boss AI Begin@@@@@@@@@@@@@@@@@"));
		});

	//�ΰ����� ���߱�.
	OnAIStopDelegate.AddLambda([this]()->void {
		GetBrainComponent()->PauseLogic("Boss AI Stop");
		UE_LOG(LogTemp, Warning, TEXT("Boss AI Stop@@@@@@@@@@@@@@@@@"));
		});
}

//Pawn�� ���ǵǾ����� ȣ��Ǵ� �Լ�. 
void AMyBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		//BT ������ ���� BB �� �ʱ�ȭ.
		Blackboard->SetValueAsInt(PhaseKey, 1); //������ 1�� �ʱ�ȭ.
		Blackboard->SetValueAsEnum(DetactRange, uint8(EDetactRange::RANGE_BIG)); //�Ÿ��� �������� �ʱ�ȭ.
		//Blackboard->SetValueAsObject(TargetActorKey, GetWorld()->GetFirstLocalPlayerFromController()); //�÷��̾� ��Ʈ�ѷ��� Ÿ�� ���ͷ� �ʱ�ȭ.

		AMyBoss* PossessedPawn = Cast<AMyBoss>(GetPawn());
		if (nullptr == PossessedPawn) return;
		PossessedPawn->OnPhaseChanged.AddLambda([this, PossessedPawn]()->void {
			UE_LOG(LogTemp, Error, TEXT("Phase Change!!!!!!!!!!!!!!!"));
			Blackboard->SetValueAsInt(PhaseKey, PossessedPawn->GetPhase()); //����� �ٲ�� �����尪 �ٲ��� ��������Ʈ.
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

	Blackboard->SetValueAsObject(TargetActorKey, GetWorld()->GetFirstPlayerController()->GetPawn()); //�÷��̾� ��Ʈ�ѷ��� Ÿ�� ���ͷ� �ʱ�ȭ.
	Blackboard->SetValueAsInt(RandNumKey, 1);
}
