// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack"); // 노드 이름 설정.
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); //지역변수 선언후 부모의 함수실행값 저장.

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // 주인 폰 받아오기.
	if (nullptr == ControllingPawn) return false; //예외처리.

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey)); // 블랙보드의 타겟값 저장.
	if (nullptr == Target) return false; // 예외처리.

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f); // 타겟과 주인폰사이에 거리를 재서 2미터보다 가까우면 true를 결과값에 저장.
	return bResult; // 결과값 반환.
}
