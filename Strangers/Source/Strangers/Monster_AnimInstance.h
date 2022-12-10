// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyMonster.h"
#include "Monster_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UMonster_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UMonster_AnimInstance();
	
	void SetDeadAnim() { IsDead = true; }
	void PlayAttackMontage();
	

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //ƽ���� ȣ��

	

private:
	//�ִϸ��̼� �������Ʈ���� ����� ���ڵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;

	UPROPERTY()
		class AMyMonster* MonsterReference;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
		
};
