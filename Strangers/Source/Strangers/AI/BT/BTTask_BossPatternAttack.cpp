// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_BossPatternAttack.h"
#include "Character/Boss/MyBoss.h"
#include "AI/MyBossAIController.h"

UBTTask_BossPatternAttack::UBTTask_BossPatternAttack()
{
	bNotifyTick = true; // �� ���� true�̸� TickTask() �Լ��� ȣ��˴ϴ�.
	EPatternToExcute = EBossPattern::BOSSPATTERN_NORMAL_ATTACK1; // ���� ���� ����Ʈ��.
}

//�׽�ũ ���� �Լ�.
EBTNodeResult::Type UBTTask_BossPatternAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); //�θ��� �Լ� ���� ���� �� ����� ����.

	auto MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // BT ������ �ҷ�����.
	if (nullptr == MyBoss) return EBTNodeResult::Failed; // ����ó��

	switch (EPatternToExcute)
	{
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK1:
		MyBoss->ExecuteNormalAttack1();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK2:
		MyBoss->ExecuteNormalAttack2();
		break;
	case EBossPattern::BOSSPATTERN_NORMAL_ATTACK3:
		MyBoss->ExecuteNormalAttack3();
		break;
	default:
		MyBoss->ExecuteNormalAttack1();
		break;
	}

	return EBTNodeResult::InProgress; // �׽�ũ�� ���������� return.
}

void UBTTask_BossPatternAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	static AMyBoss* MyBoss = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn()); // BT ������ �ҷ�����.
	if (nullptr == MyBoss) return; // ����ó��

	if (MyBoss->GetIsAttackEnded()) //Attack��Ÿ�ְ� ��������,
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//�½�ũ �����Լ�. �׽�ũ ���� ��ȯ. 
		UE_LOG(LogTemp, Error, TEXT("Task Succeeded Return"));
		return;
	}
}