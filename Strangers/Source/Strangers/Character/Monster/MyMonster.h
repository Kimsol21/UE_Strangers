// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "MyMonster.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnMonsterMeetPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonsterFartherAwayPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayerFocusOnThisDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLockOnRemoveThisDelegate);


/**
 * 
 */
UCLASS()
class STRANGERS_API AMyMonster : public AMyCharacter
{
	GENERATED_BODY()

public:
	AMyMonster();
	void Attack();

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* LockOnWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
		class UMyMonsterStatComponent* MyStat;

	//파티클설정
	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
		UParticleSystemComponent* DeadEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* DamagedEffect;

	

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);

	FOnMonsterMeetPlayerDelegate& OnMonsterMeetPlayer() { return OnMonsterMeetPlayerDelegate; };
	FOnMonsterFartherAwayPlayerDelegate& OnMonsterFartherAwayPlayer() { return OnMonsterFartherAwayPlayerDelegate; };
	FOnPlayerFocusOnThisDelegate& OnPlayerFocusOnThis() { return OnPlayerFocusOnThisDelegate; };
	FOnLockOnRemoveThisDelegate& OnLockOnRemoveThis() { return OnLockOnRemoveThisDelegate; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor 컴포넌트가 완전히 초기화된 이후 호출된다.게임 플레이 중에만 호출

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;
	

private:
	FOnMonsterMeetPlayerDelegate OnMonsterMeetPlayerDelegate;
	FOnMonsterFartherAwayPlayerDelegate OnMonsterFartherAwayPlayerDelegate;
	FOnPlayerFocusOnThisDelegate OnPlayerFocusOnThisDelegate;
	FOnLockOnRemoveThisDelegate OnLockOnRemoveThisDelegate;

	UPROPERTY()
	class UMonster_AnimInstance* MonsterAnim; //캐릭터클래스에서 애님 인스턴스는 자주 사용하므로 멤버변수로 선언한다.

	UPROPERTY()
	class AMyPlayer* CurrentAttacker; //가장 최근에 Monster에게 피해를 입힌 Actor를 저장하는 변수.

};
