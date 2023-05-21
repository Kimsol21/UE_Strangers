// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_TurnToTarget.h"
#include "AI/MyAIController.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Monster/MyMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	bNotifyTick = true; // 이 값이 true이면 TickTask() 함수가 호출됩니다.
	NodeName = TEXT("Turn"); // 노드 기본 이름 설정.
	TurnSpeed = 100.0f;
}

//테스크를 실행하는 함수.
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //부모의 테스트 결과 Result에 저장.

	//auto MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn()); // BT 소유주 찾아 저장.
	//if (nullptr == MyMonster) return EBTNodeResult::Failed; // 예외처리.

	OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	//auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey)); // 감지된 타겟 찾아 저장.
	auto Target = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (nullptr == Target) return EBTNodeResult::Failed; // 예외처리.

	FVector LookVector = Target->GetActorLocation() - OwnerPawn->GetActorLocation(); //타겟의 위치벡터에서 자신의 위치벡터를 뺀 값을 바라봐야하는 방향 벡터로 저장한다.
	LookVector.Z = 0.0f; // Z값은 높이와 관련된 값이므로 빼준다. 고개과 관련된 블랜드스페이스나 에임오프셋이 있다면 설정해주기.
	TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator(); //위에서 계산한 벡터의 Rotator를 받아와 저장한다.
	
	// RInterpTo 인자 (현재값, 목표값, 프레임 수행에 걸리는 시간, 보간 속도)

	//return EBTNodeResult::Succeeded; //테스크 성공을 알린다. 
	return EBTNodeResult::InProgress; // 테스크 진행중으로 설정.
}


void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerPawn->SetActorRotation(FMath::RInterpConstantTo(OwnerPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));// 자신의 Rotation값을 보간을 통해 설정해준다.
	if (FMath::Abs(OwnerPawn->GetActorRotation().Yaw-TargetRot.Yaw)<1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크 종료함수. 테스크 성공 반환. 
		return;
	}
	
	return;
}
