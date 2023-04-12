// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack"); // ��� �̸� ����.
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); //�������� ������ �θ��� �Լ����ప ����.

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ���� �� �޾ƿ���.
	if (nullptr == ControllingPawn) return false; //����ó��.

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey)); // �������� Ÿ�ٰ� ����.
	if (nullptr == Target) return false; // ����ó��.

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f); // Ÿ�ٰ� ���������̿� �Ÿ��� �缭 2���ͺ��� ������ true�� ������� ����.
	return bResult; // ����� ��ȯ.
}
