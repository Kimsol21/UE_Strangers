// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	OnMontageEnded.AddDynamic(this, &UMyAnimInstance::OnMyMontageEnded);
	

#pragma region Load Animation Assets
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Animations/Player/Kwang_Skeleton_Montage.Kwang_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DAMAGED_MONTAGE(TEXT("AnimMontage'/Game/Animations/Player/Kwang_Damaged_Montage.Kwang_Damaged_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		DamagedMontage = DAMAGED_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_MONTAGE(TEXT("AnimMontage'/Game/Animations/Player/Kwang_RollInplace_Montage.Kwang_RollInplace_Montage'"));
	if (ROLL_MONTAGE.Succeeded())
	{
		RollMontage = ROLL_MONTAGE.Object;
	}
#pragma endregion
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;


	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size(); //ĳ���� �ӵ� �Ǻ�.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling(); //ĳ���Ͱ� ���߿� ���ִ��� �Ǻ�.
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	Montage_Play(AttackMontage, 1.0f);//AnimInstance�� �����ϴ� �Լ�. ����:�÷����� ��Ÿ��, ����ӵ�
}

void UMyAnimInstance::PlayDamagedMontage()
{
	if (IsDead) return;
	Montage_Play(DamagedMontage, 0.01f);
}

void UMyAnimInstance::PlayRollMontage()
{
	if (IsDead) return;
	Montage_Play(RollMontage, 1.0f);
}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (IsDead) return;

	if (Montage_IsPlaying(AttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage); //���ڷ� ���� �޺��� ����FName���� ��ȯ�� AttackMontage�� �ش� ������ ���.
	}
}

void UMyAnimInstance::OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		OnAttackEnd.Broadcast();
	}
	else if(Montage == RollMontage)
	{
		OnRollEnd.Broadcast();
	}
	else
	{
		return;
	}
}

void UMyAnimInstance::AnimNotify_AttackHitCheck() //�ش� ��Ƽ���̰� �߻��ϸ�, 
{
	OnAttackHitCheck.Broadcast(); //������ ��������Ʈ�� ���ε��� �Լ��� ����(Broadcast).
}

void UMyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (!FMath::IsWithinInclusive<int32>(Section, 1, 4))
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong input paramater error!!!!!"));
	}
	return FName(*FString::Printf(TEXT("Attack%d"), Section)); //�𸮾��� Ư���� ���ڿ� ��Ʈ�ѹ��.
}
