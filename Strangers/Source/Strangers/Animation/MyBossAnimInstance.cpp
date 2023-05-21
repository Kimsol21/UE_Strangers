// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MyBossAnimInstance.h"
#include "Character/Boss/MyBoss.h"


UMyBossAnimInstance::UMyBossAnimInstance()
{
	CurrentPawnSpeed = 0.0f;


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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_NORMAL_ATTACK3(TEXT("AnimMontage'/Game/Animations/Boss/Boss_JumpAttack_Montage.Boss_JumpAttack_Montage'"));
	if (MONTAGE_NORMAL_ATTACK3.Succeeded())
	{
		NormalAttack3_Montage = MONTAGE_NORMAL_ATTACK3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_KICK(TEXT("AnimMontage'/Game/Animations/Boss/Boss_Kick_Montage.Boss_Kick_Montage'"));
	if (MONTAGE_KICK.Succeeded())
	{
		Kick_Montage = MONTAGE_KICK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_STAB(TEXT("AnimMontage'/Game/Animations/Boss/Boss_Stab_Montage.Boss_Stab_Montage'"));
	if (MONTAGE_STAB.Succeeded())
	{
		Stab_Montage = MONTAGE_STAB.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_DEAD(TEXT("AnimMontage'/Game/Animations/Boss/BossDead_Montage.BossDead_Montage'"));
	if (MONTAGE_DEAD.Succeeded())
	{
		Dead_Montage = MONTAGE_DEAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MONTAGE_COMBO_ATTACK(TEXT("AnimMontage'/Game/Animations/Boss/BossNormalAttack4_Montage.BossNormalAttack4_Montage'"));
	if (MONTAGE_COMBO_ATTACK.Succeeded())
	{
		ComboAttack_Montage = MONTAGE_COMBO_ATTACK.Object;
	}

	IsDead = false;
}


void UMyBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	MyBoss = Cast<AMyBoss>(Pawn);
	if (!MyBoss) return;

	if (MyBoss)
	{
		MyBoss->OnBossHPIsZero().AddLambda([this]()->void {
			//Montage_Play(Dead_Montage, 1.0f);
			IsDead = true;
			});
	}

}


void UMyBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsDead)
	{
		if (!::IsValid(MyBoss)) return;
		CurrentPawnSpeed = MyBoss->GetVelocity().Size(); //캐릭터 속도 판별.

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

void UMyBossAnimInstance::MontagePlayKick()
{
	Montage_Play(Kick_Montage, 1.0f);
}

void UMyBossAnimInstance::MontagePlayStab()
{
	Montage_Play(Stab_Montage, 1.0f);
}

void UMyBossAnimInstance::MontagePlayComboAttack()
{
	Montage_Play(ComboAttack_Montage, 1.0f);
}

void UMyBossAnimInstance::OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted)//AnimInstance의 OnMontageEnded 델리게이트 바인딩 함수.
{
	//if (Montage == Dead_Montage)//보스 죽음 몽타주 재생이 끝나면
	//{
	//	if (auto ComponentOwner = TryGetPawnOwner())
	//	{
	//		ComponentOwner->Destroy();
	//		UE_LOG(LogTemp, Warning, TEXT("BossDeadMontage Ended!!!!!!!!!!!!"));
	//	}
	//}
	UE_LOG(LogTemp, Warning, TEXT("OnBossAttackEndDelegate.Broadcast();!!!!!!!!!!!!"));
	OnBossAttackEndDelegate.Broadcast();
	/*else
	{
		OnBossAttackEnd.Broadcast();
	}*/
	
}

void UMyBossAnimInstance::AnimNotify_Notify_SetFlyingMode()
{
	OnChangeFlyingMode.Broadcast();
}

void UMyBossAnimInstance::AnimNotify_Notify_SetWalkingMode()
{
	OnChangeWalkingMode.Broadcast();
}

void UMyBossAnimInstance::AnimNotify_Notify_ExcuteJump()
{
	OnExcuteBossJumpEvent.Broadcast();
}

void UMyBossAnimInstance::AnimNotify_AttackHitCheck() //해당 노티파이가 발생하면, 
{
	OnAttackCheckEvent.Broadcast(); //선언한 델리게이트에 바인딩된 함수들 실행(Broadcast).
}

void UMyBossAnimInstance::AnimNotify_EffectActivate() //해당 노티파이가 발생하면, 
{
	OnEffectActivateEvent.Broadcast(); //선언한 델리게이트에 바인딩된 함수들 실행(Broadcast).
}