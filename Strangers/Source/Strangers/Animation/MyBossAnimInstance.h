// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyBossAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnBossAttackEndDelegate); //���� ������ ȣ���� ��������Ʈ.
DECLARE_MULTICAST_DELEGATE(FOnChangeFlyingModeDelegate); // CharacterMovementMode�� FlyingMode�� �ٲ�� �ϴ� ��Ȳ�϶�  ex) Animation RootMotion�� z�� �̵��� ���Ե� ���.
DECLARE_MULTICAST_DELEGATE(FOnChangeWalkingModeDelegate); //���� ��Ȳ���� ����ġ��ų�� ȣ��.

UCLASS()
class STRANGERS_API UMyBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMyBossAnimInstance();

	void MontagePlayNormalAttack1();
	void MontagePlayNormalAttack2();
	void MontagePlayNormalAttack3();

	FOnBossAttackEndDelegate OnBossAttackEnd;
	FOnChangeFlyingModeDelegate OnChangeFlyingMode;
	FOnChangeWalkingModeDelegate OnChangeWalkingMode;
	
protected:

private:
	//UFUNCTION()
	//void AnimNotify_AttackHitCheck(); // "AnimNotify_��Ƽ���̸�" �̸��� ��� �Լ��� ã�Ƽ� ȣ���ϴ� ���.

	UFUNCTION()
	void OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted);//AnimInstance�� OnMontageEnded ��������Ʈ ���ε� �Լ�.

	UPROPERTY()
	class AMyMonster* MonsterReference;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack1_Montage;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack2_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack3_Montage;

	UFUNCTION()
	void AnimNotify_Notify_SetFlyingMode();

	UFUNCTION()
	void AnimNotify_Notify_SetWalkingMode();

};
