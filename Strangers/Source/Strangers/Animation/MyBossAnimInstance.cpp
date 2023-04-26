// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MyBossAnimInstance.h"


UMyBossAnimInstance::UMyBossAnimInstance()
{
	OnMontageEnded.AddDynamic(this, &UMyBossAnimInstance::OnMyMontageEnded);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_NORMAL_ATTACK1(TEXT("AnimMontage'/Game/Animations/Boss/BossNormalAttack1_Montage.BossNormalAttack1_Montage'"));
	if (MONTAGE_NORMAL_ATTACK1.Succeeded())
	{
		NormalAttack1_Montage = MONTAGE_NORMAL_ATTACK1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_NORMAL_ATTACK2(TEXT("AnimMontage'/Game/Animations/Boss/BossNormalAttack2_Montage.BossNormalAttack2_Montage'"));
	if (MONTAGE_NORMAL_ATTACK2.Succeeded())
	{
		NormalAttack2_Montage = MONTAGE_NORMAL_ATTACK2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_NORMAL_ATTACK3(TEXT("AnimMontage'/Game/Animations/Boss/BossJumpAttack1_Montage.BossJumpAttack1_Montage'"));
	if (MONTAGE_NORMAL_ATTACK3.Succeeded())
	{
		NormalAttack3_Montage = MONTAGE_NORMAL_ATTACK3.Object;
	}
}

void UMyBossAnimInstance::MontagePlayNormalAttack1()
{
	Montage_Play(NormalAttack1_Montage, 1.0f);
}

void UMyBossAnimInstance::MontagePlayNormalAttack2()
{
	Montage_Play(NormalAttack2_Montage, 1.0f);
}

void UMyBossAnimInstance::MontagePlayNormalAttack3()
{
	Montage_Play(NormalAttack3_Montage, 1.0f);
}

void UMyBossAnimInstance::OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted)//AnimInstance의 OnMontageEnded 델리게이트 바인딩 함수.
{
	OnBossAttackEnd.Broadcast();
}

void UMyBossAnimInstance::AnimNotify_Notify_SetFlyingMode()
{
	OnChangeFlyingMode.Broadcast();
}

void UMyBossAnimInstance::AnimNotify_Notify_SetWalkingMode()
{
	OnChangeWalkingMode.Broadcast();
}