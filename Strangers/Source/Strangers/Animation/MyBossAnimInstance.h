// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyBossAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnBossAttackEndDelegate); //공격 끝나면 호출할 델리게이트.
DECLARE_EVENT(UMyBossAnimInstance, FOnAttackCheckEvent);//공격 체크시 호출될 델리게이트.
DECLARE_EVENT(UMyBossAnimInstance, FOnEffectActivateEvent);//공격 체크시 호출될 델리게이트.

DECLARE_MULTICAST_DELEGATE(FOnChangeFlyingModeDelegate); // CharacterMovementMode를 FlyingMode로 바꿔야 하는 상황일때  ex) Animation RootMotion이 z값 이동이 포함된 경우.
DECLARE_MULTICAST_DELEGATE(FOnChangeWalkingModeDelegate); //위의 상황에서 원위치시킬때 호출.
DECLARE_EVENT(UMyBossAnimInstance, FOnExcuteBossJumpEvent); // 보스가 점프해야할 때 호출되는 이벤트.

UCLASS()
class STRANGERS_API UMyBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMyBossAnimInstance();
	

	void MontagePlayNormalAttack1();
	void MontagePlayNormalAttack2();
	void MontagePlayNormalAttack3();
	void MontagePlayKick();
	void MontagePlayStab();
	void MontagePlayComboAttack();

	//FOnBossAttackEndDelegate OnBossAttackEnd;
	FOnChangeFlyingModeDelegate OnChangeFlyingMode;
	FOnChangeWalkingModeDelegate OnChangeWalkingMode;
	
	FOnBossAttackEndDelegate& OnBossAttackEnd() { return OnBossAttackEndDelegate; };
	FOnExcuteBossJumpEvent& OnExcuteBossJump() { return OnExcuteBossJumpEvent; };
	FOnAttackCheckEvent& OnAttackCheck() { return OnAttackCheckEvent; };
	FOnEffectActivateEvent& OnEffectActivate() { return OnEffectActivateEvent; };
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //틱마다 호출
private:
	FOnExcuteBossJumpEvent OnExcuteBossJumpEvent;
	FOnBossAttackEndDelegate OnBossAttackEndDelegate;
	FOnAttackCheckEvent OnAttackCheckEvent;
	FOnEffectActivateEvent OnEffectActivateEvent;
	
	UFUNCTION()
	void AnimNotify_AttackHitCheck(); // "AnimNotify_노티파이명" 이름의 멤버 함수를 찾아서 호출하는 방식.

	UFUNCTION()
	void AnimNotify_EffectActivate();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UFUNCTION()
	void OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted);//AnimInstance의 OnMontageEnded 델리게이트 바인딩 함수.

	UPROPERTY()
	class AMyMonster* MonsterReference;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack1_Montage;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack2_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttack3_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Kick_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Stab_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Dead_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ComboAttack_Montage;

	UFUNCTION()
	void AnimNotify_Notify_SetFlyingMode();

	UFUNCTION()
	void AnimNotify_Notify_SetWalkingMode();

	UFUNCTION()
	void AnimNotify_Notify_ExcuteJump();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	class AMyBoss* MyBoss;

};
