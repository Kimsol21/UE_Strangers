// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate); //공격 끝나면 호출할 델리게이트

UCLASS()
class STRANGERS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//액터가 받은 대미지를 처리하는 로직을 추가하기 위해 오버라이드.
	virtual void AttackCheck();//물리엔진으로 공격감지.

	virtual void SetDamage(float _Damage) { return; };
	virtual float GetAttackPower() { return 0.0f; };

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;//공격 감지 구가 지나갈 길이 (공격길이)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;//공격 감지 구의 반지름(공격 범위)
};
