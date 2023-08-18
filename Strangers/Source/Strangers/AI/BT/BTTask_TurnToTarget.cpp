#include "AI/BT/BTTask_TurnToTarget.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Monster/MyMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	bNotifyTick = true; 
	NodeName = TEXT("Turn"); 
	TurnSpeed = 100.0f;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto Target = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (nullptr == Target) return EBTNodeResult::Failed; // 예외처리.

	FVector LookVector = Target->GetActorLocation() - OwnerPawn->GetActorLocation(); 
	LookVector.Z = 0.0f; // 고개과 관련된 블랜드스페이스나 에임오프셋이 있다면 설정해주기.
	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); 

	return EBTNodeResult::InProgress; // 테스크 진행중으로 설정.
}


void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerPawn->SetActorRotation(FMath::RInterpConstantTo(OwnerPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));
	if (FMath::Abs(OwnerPawn->GetActorRotation().Yaw-TargetRot.Yaw)<1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	return;
}
