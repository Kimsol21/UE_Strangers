// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);//멀티캐스트 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class STRANGERS_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //틱마다 호출

	void PlayAttackMontage();
	void PlayDamagedMontage();
	void JumpToAttackMontageSection(int32 NewSection);//콤보에 따른 몽타주 섹션 넘기기

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;//위에서 선언한 델리게이트 유형으로 만든 델리게이트.
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck(); // "AnimNotify_노티파이명" 이름의 멤버 함수를 찾아서 호출하는 방식.

	UFUNCTION()
		void AnimNotify_NextAttackCheck(); 

	FName GetAttackMontageSectionName(int32 Section); //콤보수를 입력받아 해당 몽타주섹션 이름을 반환해주는 함수.

private:
	//애니메이션 블루프린트에서 사용할 인자들.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
	
};
