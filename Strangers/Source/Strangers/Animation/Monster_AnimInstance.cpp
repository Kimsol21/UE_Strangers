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
		CurrentPawnSpeed = MonsterReference->GetVelocity().Size();
	}
}

void UMonster_AnimInstance::AnimNotify_AttackHitCheck() 
{
	OnAttackHitCheck.Broadcast();
}

void UMonster_AnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	
	Montage_Play(AttackMontage, 1.0f);
}

void UMonster_AnimInstance::PlayDamagedMontage()
{
	if (IsDead) return;

	Montage_Play(DamagedMontage, 1.3f);
}
