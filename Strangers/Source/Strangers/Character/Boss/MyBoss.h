// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBoss.generated.h"

class UWidgetComponent;
class UMyBossAnimInstance;

//DECLARE_EVENT_OneParam(AMyBoss, FBossHPChangedEvent, float);

DECLARE_MULTICAST_DELEGATE(FOnPhaseChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBossHPIsZeroDelegate);

DECLARE_EVENT_OneParam(AMyBoss, FBossHPChangedEvent, float);

UCLASS()
class STRANGERS_API AMyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	AMyBoss();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��

	//�ǰ� ���� �Լ�.
	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetDamage(float _Damage) override;
	virtual float GetAttackPower() override;*/
public:	
	FBossHPChangedEvent& OnBossHPChanged() { return BossHPChangedEvent; }



	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//���Ͱ� ���� ������� ó���ϴ� ������ �߰��ϱ� ���� �������̵�.

	//�������� ���� �Լ�.
	//void PerformAttackPattern_1(); 



public:
	uint8 GetPhase() const { return Phase; };
	bool GetIsAttackEnded() const { return bIsAttackEnded; };
	//bool GetIsFighting() const { return bIsFighting; };
	void SetIsFighting(bool _bIsFighting) { bIsFighting = _bIsFighting; };

	void ExecuteNormalAttack1();
	void ExecuteNormalAttack2();
	void ExecuteNormalAttack3();

	//UPROPERTY(Category = UI)
	//UWidgetComponent* BossHUDWidget; // ������ ���� HUD.

	//����Ʈ ����.
	/*UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	UParticleSystemComponent* DeadEffect; 

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);*/

	FOnBossHPIsZeroDelegate OnBossHPIsZero;
	FOnPhaseChangedDelegate OnPhaseChanged;

private:
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* LockOnWidget;

	bool bIsFighting; // �������� ���۵Ǿ�����.

	FBossHPChangedEvent BossHPChangedEvent;

	void AttackCheck(); //OnAttackCheck ��������Ʈ���� ȣ���� �Լ�.
	
	UPROPERTY()
	UMyBossAnimInstance* BossAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.

	static const float MaxHP;
	float CurrentHP;
	float AttackPower;//���� �Ŀ�
	float AttackRange;//���� ���� ���� ������ ���� (���ݱ���)
	float AttackRadius;//���� ���� ���� ������(���� ����)
	
	bool bIsAttackEnded = true;
	
	uint8 Phase = 1;

	void SetHP(const float& _NewHP);


	//UPROPERTY()
	//class AMyPlayer* CurrentAttacker; //���� �ֱٿ� Monster���� ���ظ� ���� Actor�� �����ϴ� ����.

};