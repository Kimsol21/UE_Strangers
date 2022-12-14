// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
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
	if (!::IsValid(Pawn)) return;


	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size(); //캐릭터 속도 판별.
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling(); //캐릭터가 공중에 떠있는지 판별.
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsDead) return;

	Montage_Play(AttackMontage, 1.0f);//AnimInstance가 제공하는 함수. 인자:플레이할 몽타주, 재생속도
}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (IsDead) return;

	if (Montage_IsPlaying(AttackMontage))
	{
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage); //인자로 받은 콤보를 섹션FName으로 변환해 AttackMontage의 해당 섹션을 재생.
	}
}

void UMyAnimInstance::AnimNotify_AttackHitCheck() //해당 노티파이가 발생하면, 
{
	OnAttackHitCheck.Broadcast(); //선언한 델리게이트에 바인딩된 함수들 실행(Broadcast).
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
	return FName(*FString::Printf(TEXT("Attack%d"), Section)); //언리얼의 특별한 문자열 컨트롤방식.
}
