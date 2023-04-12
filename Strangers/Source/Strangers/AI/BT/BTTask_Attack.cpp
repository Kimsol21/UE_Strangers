// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_Attack.h"
#include "AI/MyAIController.h"
#include "Character/Monster/MyMonster.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; // �� ���� true�̸� TickTask() �Լ��� ȣ��˴ϴ�.
	IsAttacking = false; // ��������� ����� ���������� �Ǵ��ϴ� ���� false �ʱ�ȭ.
}

//�׽�ũ ���� �Լ�.
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //�θ��� �Լ� ���� ���� �� ����� ����.

	auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn()); // BT ������ �ҷ�����.
	if (nullptr == MyMonster) return EBTNodeResult::Failed; // ����ó��

	MyMonster->Attack(); // ������ �ҷ��� �������� �����Լ� ȣ��.
	IsAttacking = true; // �������� true�� ����.

	return EBTNodeResult::InProgress; // �׽�ũ�� ���������� return.
}

//�׽�ũ Tick �Լ�.
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) //�������� �ƴ϶�� �׽�ũ ����.
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//�½�ũ �����Լ�. �׽�ũ ���� ��ȯ. 
	}
}
