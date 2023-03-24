// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);//��Ƽĳ��Ʈ ��������Ʈ ����.
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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //ƽ���� ȣ��

	void PlayAttackMontage();
	void PlayDamagedMontage();
	void JumpToAttackMontageSection(int32 NewSection);//�޺��� ���� ��Ÿ�� ���� �ѱ��

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;//������ ������ ��������Ʈ �������� ���� ��������Ʈ.
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck(); // "AnimNotify_��Ƽ���̸�" �̸��� ��� �Լ��� ã�Ƽ� ȣ���ϴ� ���.

	UFUNCTION()
		void AnimNotify_NextAttackCheck(); 

	FName GetAttackMontageSectionName(int32 Section); //�޺����� �Է¹޾� �ش� ��Ÿ�ּ��� �̸��� ��ȯ���ִ� �Լ�.

private:
	//�ִϸ��̼� �������Ʈ���� ����� ���ڵ�.
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
