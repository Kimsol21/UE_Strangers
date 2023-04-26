// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBoss.generated.h"

class UWidgetComponent;
class UMyBossAnimInstance;

DECLARE_MULTICAST_DELEGATE(FOnBossHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPhaseChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBossHPIsZeroDelegate);

UCLASS()
class STRANGERS_API AMyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	AMyBoss();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출

	//피격 관련 함수.
	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;*/
public:	
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//액터가 받은 대미지를 처리하는 로직을 추가하기 위해 오버라이드.

	//공격패턴 관련 함수.
	//void PerformAttackPattern_1(); 

public:
	uint8 GetPhase() const { return Phase; };
	bool GetIsAttackEnded() const { return bIsAttackEnded; };

	void ExecuteNormalAttack1();
	void ExecuteNormalAttack2();
	void ExecuteNormalAttack3();

	//UPROPERTY(Category = UI)
	//UWidgetComponent* BossHUDWidget; // 보스전 관련 HUD.

	//이펙트 관련.
	/*UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	UParticleSystemComponent* DeadEffect; 

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);*/

	FOnBossHPChangedDelegate OnBossHPChanged;
	FOnBossHPIsZeroDelegate OnBossHPIsZero;
	FOnPhaseChangedDelegate OnPhaseChanged;

private:
	void AttackCheck(); //OnAttackCheck 델리게이트에서 호출할 함수.
	
	UPROPERTY()
	UMyBossAnimInstance* BossAnim; //캐릭터클래스에서 애님 인스턴스는 자주 사용하므로 멤버변수로 선언한다.

	static const float MaxHP;
	float CurrentHP;
	float AttackPower;//공격 파워
	float AttackRange;//공격 감지 구가 지나갈 길이 (공격길이)
	float AttackRadius;//공격 감지 구의 반지름(공격 범위)
	
	bool bIsAttackEnded = true;
	
	uint8 Phase = 1;

	void SetHP(const float& _NewHP);


	//UPROPERTY()
	//class AMyPlayer* CurrentAttacker; //가장 최근에 Monster에게 피해를 입힌 Actor를 저장하는 변수.

};
