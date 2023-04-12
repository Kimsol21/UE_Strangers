// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_Attack.h"
#include "AI/MyAIController.h"
#include "Character/Monster/MyMonster.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; // 이 값이 true이면 TickTask() 함수가 호출됩니다.
	IsAttacking = false; // 멤버변수로 선언된 공격중인지 판단하는 변수 false 초기화.
}

//테스크 실행 함수.
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //부모의 함수 먼저 실행 후 결과값 저장.

	auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn()); // BT 소유주 불러오기.
	if (nullptr == MyMonster) return EBTNodeResult::Failed; // 예외처리

	MyMonster->Attack(); // 위에서 불러온 소유주의 공격함수 호출.
	IsAttacking = true; // 공격중을 true로 설정.

	return EBTNodeResult::InProgress; // 테스크가 진행중임을 return.
}

//테스크 Tick 함수.
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) //공격중이 아니라면 테스크 종료.
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크 종료함수. 테스크 성공 반환. 
	}
}
