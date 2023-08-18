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
	if (nullptr == Target) return EBTNodeResult::Failed; // ����ó��.

	FVector LookVector = Target->GetActorLocation() - OwnerPawn->GetActorLocation(); 
	LookVector.Z = 0.0f; // ���� ���õ� ���彺���̽��� ���ӿ������� �ִٸ� �������ֱ�.
	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); 

	return EBTNodeResult::InProgress; // �׽�ũ ���������� ����.
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
