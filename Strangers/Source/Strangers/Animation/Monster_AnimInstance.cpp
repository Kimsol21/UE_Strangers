// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_AnimInstance.h"
#include "Character/Monster/MyMonster.h"


UMonster_AnimInstance::UMonster_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Animations/Monster/Corpse_Skeleton_Montage.Corpse_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAMAGED_MONTAGE(TEXT("AnimMontage'/Game/Animations/Monster/Damaged_Montage.Damaged_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		DamagedMontage = DAMAGED_MONTAGE.Object;
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
		CurrentPawnSpeed = MonsterReference->GetVelocity().Size(); //캐릭터 속도 판별.

	}
}

void UMonster_AnimInstance::AnimNotify_AttackHitCheck() //해당 노티파이가 발생하면, 
{
	OnAttackHitCheck.Broadcast(); //선언한 델리게이트에 바인딩된 함수들 실행(Broadcast).
}

void UMonster_AnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	
	Montage_Play(AttackMontage, 1.0f);//AnimInstance가 제공하는 함수. 인자:플레이할 몽타주, 재생속도
}

void UMonster_AnimInstance::PlayDamagedMontage()
{
	if (IsDead) return;

	Montage_Play(DamagedMontage, 1.3f);
}
