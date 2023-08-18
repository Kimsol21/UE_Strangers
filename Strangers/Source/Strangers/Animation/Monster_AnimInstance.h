#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Monster_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class STRANGERS_API UMonster_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonster_AnimInstance();
	
	void SetDeadAnim() { IsDead = true; }
	void RestartAnimLogic() { IsDead = false; };
	void PlayAttackMontage();
	void PlayDamagedMontage();

	FOnAttackHitCheckDelegate OnAttackHitCheck;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; 

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck(); // "AnimNotify_노티파이명" 이름의 멤버 함수를 찾아서 호출하는 방식.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY()
	class AMyMonster* MonsterReference;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DamagedMontage;		
};
