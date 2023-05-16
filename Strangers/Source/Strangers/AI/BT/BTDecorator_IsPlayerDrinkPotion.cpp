// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTDecorator_IsPlayerDrinkPotion.h"
#include "AI/MyBossAIController.h"
#include "Character/Player/MyPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsPlayerDrinkPotion::UBTDecorator_IsPlayerDrinkPotion()
{
	NodeName = TEXT("InProgress"); // 노드 이름 설정.
}

bool UBTDecorator_IsPlayerDrinkPotion::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMyPlayer* Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyBossAIController::TargetActorKey));
	if (nullptr == Target) return false;

	//UE_LOG(LogTemp, Error, TEXT("This is a Test : %d"), Target->GetIsDrinkPotion());

	return Target->GetIsDrinkPotion();
}
