// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_TurnToTarget.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Monster/MyMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	bNotifyTick = true; // �� ���� true�̸� TickTask() �Լ��� ȣ��˴ϴ�.
	NodeName = TEXT("Turn"); // ��� �⺻ �̸� ����.
	TurnSpeed = 100.0f;
}

//�׽�ũ�� �����ϴ� �Լ�.
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //�θ��� �׽�Ʈ ��� Result�� ����.

	//auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn()); // BT ������ ã�� ����.
	//if (nullptr == MyMonster) return EBTNodeResult::Failed; // ����ó��.

	OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	//auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey)); // ������ Ÿ�� ã�� ����.
	auto Target = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (nullptr == Target) return EBTNodeResult::Failed; // ����ó��.

	FVector LookVector = Target->GetActorLocation() - OwnerPawn->GetActorLocation(); //Ÿ���� ��ġ���Ϳ��� �ڽ��� ��ġ���͸� �� ���� �ٶ�����ϴ� ���� ���ͷ� �����Ѵ�.
	LookVector.Z = 0.0f; // Z���� ���̿� ���õ� ���̹Ƿ� ���ش�. ���� ���õ� ���彺���̽��� ���ӿ������� �ִٸ� �������ֱ�.
	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //������ ����� ������ Rotator�� �޾ƿ� �����Ѵ�.
	
	// RInterpTo ���� (���簪, ��ǥ��, ������ ���࿡ �ɸ��� �ð�, ���� �ӵ�)

	//return EBTNodeResult::Succeeded; //�׽�ũ ������ �˸���. 
	return EBTNodeResult::InProgress; // �׽�ũ ���������� ����.
}


void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerPawn->SetActorRotation(FMath::RInterpConstantTo(OwnerPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));// �ڽ��� Rotation���� ������ ���� �������ش�.
	if (FMath::Abs(OwnerPawn->GetActorRotation().Yaw-TargetRot.Yaw)<1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//�½�ũ �����Լ�. �׽�ũ ���� ��ȯ. 
		return;
	}
	
	return;
}
