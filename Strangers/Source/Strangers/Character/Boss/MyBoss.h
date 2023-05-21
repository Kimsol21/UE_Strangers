// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBoss.generated.h"

class UWidgetComponent;
class UMyBossAnimInstance;


// 보스 상태에 관한 Enum.
UENUM(BlueprintType)
enum class EBossState : uint8
{
	NORMALL UMETA(DisplayName = "Normal"),
	ATTACKING_START UMETA(DisplayName = "AttackingStart"),
	ATTACKING_END UMETA(DisplayName = "AttackingEnd")

};


//DECLARE_EVENT_OneParam(AMyBoss, FBossHPChangedEvent, float);

DECLARE_MULTICAST_DELEGATE(FOnPhaseChangedDelegate);

DECLARE_EVENT(AMyBoss, FOnBossHPIsZeroEvent);
DECLARE_EVENT_OneParam(AMyBoss, FBossHPChangedEvent, float);

UCLASS()
class STRANGERS_API AMyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	AMyBoss();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출


public:	
	FBossHPChangedEvent& OnBossHPChanged() { return BossHPChangedEvent; }

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//액터가 받은 대미지를 처리하는 로직을 추가하기 위해 오버라이드.

	//공격패턴 관련 함수.
	//void PerformAttackPattern_1(); 
	EBossState BossState;
public:
	uint8 GetPhase() const { return Phase; };
	bool GetIsAttackEnded() const { return bIsAttackEnded; };
	//bool GetIsFighting() const { return bIsFighting; };
	//id SetIsFighting(bool _bIsFighting) { bIsFighting = _bIsFighting; };

	void ExecuteNormalAttack1();
	void ExecuteNormalAttack2();
	void ExecuteNormalAttack3();
	void ExecuteStab();
	void ExecuteKick();
	void ExecuteComboAttack();

	//이펙트 관련.
	/*UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	UParticleSystemComponent* DeadEffect; 

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);*/

	
	FOnPhaseChangedDelegate OnPhaseChanged;

	FOnBossHPIsZeroEvent& OnBossHPIsZero() { return OnBossHPIsZeroEvent; };
	class ULockOnComponent& GetLockOnComponent() const { return *LockOnComponent; };
private:
	USkeletalMeshComponent* BossWeaponMesh;

	FOnBossHPIsZeroEvent OnBossHPIsZeroEvent;

	UPROPERTY(VisibleAnywhere, Category = LockOn)
	class ULockOnComponent* LockOnComponent;

	//bool bIsFighting; // 보스전이 시작되었는지.

	FBossHPChangedEvent BossHPChangedEvent;

	void AttackCheck(); //OnAttackCheck 델리게이트에서 호출할 함수.

	void JumpToTarget(float _JumpPower);
	
	UPROPERTY()
	UMyBossAnimInstance* BossAnim; //캐릭터클래스에서 애님 인스턴스는 자주 사용하므로 멤버변수로 선언한다.

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* DeadEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* JumpAttackEffect;

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);

	static const float MaxHP;
	float CurrentHP;
	float AttackPower;//공격 파워
	float AttackRange;//공격 감지 구가 지나갈 길이 (공격길이)
	float AttackRadius;//공격 감지 구의 반지름(공격 범위)
	
	bool bIsAttackEnded;

	
	uint8 Phase = 1;

	void SetHP(const float& _NewHP);

	//UPROPERTY()
	//class AMyPlayer* CurrentAttacker; //가장 최근에 Monster에게 피해를 입힌 Actor를 저장하는 변수.

	class AMyBossAIController* BossAIController;

};
