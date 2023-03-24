// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "MyMonster.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
		class UMyMonsterStatComponent* MyStat;

	//��ƼŬ����
	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
		UParticleSystemComponent* DeadEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* DamagedEffect;

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;
	

private:
	UPROPERTY()
	class UMonster_AnimInstance* MonsterAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.

	UPROPERTY()
	class AMyPlayer* CurrentAttacker; //���� �ֱٿ� Monster���� ���ظ� ���� Actor�� �����ϴ� ����.

};
