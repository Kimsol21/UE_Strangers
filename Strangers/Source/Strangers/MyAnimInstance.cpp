// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Animations/Kwang_Skeleton_Montage.Kwang_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);//AnimInstance�� �����ϴ� �Լ�. ����:�÷����� ��Ÿ��, ����ӵ�
}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(AttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage); //���ڷ� ���� �޺��� ����FName���� ��ȯ�� AttackMontage�� �ش� ������ ���.
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
