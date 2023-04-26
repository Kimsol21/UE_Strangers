// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_TurnToTarget.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Monster/MyMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn"); // ��� �⺻ �̸� ����.
}

//�׽�ũ�� �����ϴ� �Լ�.
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //�θ��� �׽�Ʈ ��� Result�� ����.

	auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn()); // BT ������ ã�� ����.
	if (nullptr == MyMonster) return EBTNodeResult::Failed; // ����ó��.

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey)); // ������ Ÿ�� ã�� ����.
	if (nullptr == Target) return EBTNodeResult::Failed; // ����ó��.

	FVector LookVector = Target->GetActorLocation() - MyMonster->GetActorLocation(); //Ÿ���� ��ġ���Ϳ��� �ڽ��� ��ġ���͸� �� ���� �ٶ�����ϴ� ���� ���ͷ� �����Ѵ�.
	LookVector.Z = 0.0f; // Z���� ���̿� ���õ� ���̹Ƿ� ���ش�. ���� ���õ� ���彺���̽��� ���ӿ������� �ִٸ� �������ֱ�.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //������ ����� ������ Rotator�� �޾ƿ� �����Ѵ�.
	MyMonster->SetActorRotation(FMath::RInterpTo(MyMonster->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));// �ڽ��� Rotation���� ������ ���� �������ش�.
	// RInterpTo ���� (���簪, ��ǥ��, ������ ���࿡ �ɸ��� �ð�, ���� �ӵ�)

	return EBTNodeResult::Succeeded; //�׽�ũ ������ �˸���. 
}
