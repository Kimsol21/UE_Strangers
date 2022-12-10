// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_AnimInstance.h"


UMonster_AnimInstance::UMonster_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Animations/Corpse_Skeleton_Montage.Corpse_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UMonster_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MonsterReference = Cast<AMyMonster>(TryGetPawnOwner());
	if (!::IsValid(MonsterReference)) return;

	
}

void UMonster_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsDead)
	{
		if (!::IsValid(MonsterReference)) return;
		CurrentPawnSpeed = MonsterReference->GetVelocity().Size(); //ĳ���� �ӵ� �Ǻ�.
		//auto Character = Cast<ACharacter>(Pawn);
		/*if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling(); //ĳ���Ͱ� ���߿� ���ִ��� �Ǻ�.
		}*/
	}
}

void UMonster_AnimInstance::PlayAttackMontage()
{
	if (IsDead) return;

	Montage_Play(AttackMontage, 1.0f);//AnimInstance�� �����ϴ� �Լ�. ����:�÷����� ��Ÿ��, ����ӵ�
}
