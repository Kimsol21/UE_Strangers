// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "MyMonster.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnMonsterMeetPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMonsterFartherAwayPlayerDelegate);
//DECLARE_MULTICAST_DELEGATE(FOnPlayerFocusOnThisDelegate);
//DECLARE_MULTICAST_DELEGATE(FOnLockOnRemoveThisDelegate);


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

	/*UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* LockOnWidget;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
		class UMyMonsterStatComponent* MyStat;

	//��ƼŬ����
	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
		UParticleSystemComponent* DeadEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* DamagedEffect;

	

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);

	FOnMonsterMeetPlayerDelegate& OnMonsterMeetPlayer() { return OnMonsterMeetPlayerDelegate; };
	FOnMonsterFartherAwayPlayerDelegate& OnMonsterFartherAwayPlayer() { return OnMonsterFartherAwayPlayerDelegate; };
	/*FOnPlayerFocusOnThisDelegate& OnPlayerFocusOnThis() { return OnPlayerFocusOnThisDelegate; };
	FOnLockOnRemoveThisDelegate& OnLockOnRemoveThis() { return OnLockOnRemoveThisDelegate; };*/

	class ULockOnComponent& GetLockOnComponent() const{ return *LockOnComponent; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;
	

private:
	void OnMonsterRespawn(); // ���Ͱ� �����ɶ� ȣ��Ǵ� �Լ�.
	void OnMonsterDead(); // ���Ͱ� ������� �� ȣ��Ǵ� �Լ�.

	FOnMonsterMeetPlayerDelegate OnMonsterMeetPlayerDelegate;
	FOnMonsterFartherAwayPlayerDelegate OnMonsterFartherAwayPlayerDelegate;
	/*FOnPlayerFocusOnThisDelegate OnPlayerFocusOnThisDelegate;
	FOnLockOnRemoveThisDelegate OnLockOnRemoveThisDelegate;*/

	float RespawnTime;

	UPROPERTY()
	class UMonster_AnimInstance* MonsterAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.

	UPROPERTY()
	class AMyPlayer* CurrentAttacker; //���� �ֱٿ� Monster���� ���ظ� ���� Actor�� �����ϴ� ����.

	UPROPERTY()
		class AMyAIController* MyAIController;

	UPROPERTY(VisibleAnywhere, Category = LockOn)
	class ULockOnComponent* LockOnComponent;

};
