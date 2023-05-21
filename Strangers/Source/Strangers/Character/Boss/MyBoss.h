// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBoss.generated.h"

class UWidgetComponent;
class UMyBossAnimInstance;


// ���� ���¿� ���� Enum.
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
	virtual void PostInitializeComponents() override; //Actor ������Ʈ�� ������ �ʱ�ȭ�� ���� ȣ��ȴ�.���� �÷��� �߿��� ȣ��


public:	
	FBossHPChangedEvent& OnBossHPChanged() { return BossHPChangedEvent; }

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;//���Ͱ� ���� ������� ó���ϴ� ������ �߰��ϱ� ���� �������̵�.

	//�������� ���� �Լ�.
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

	//����Ʈ ����.
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

	//bool bIsFighting; // �������� ���۵Ǿ�����.

	FBossHPChangedEvent BossHPChangedEvent;

	void AttackCheck(); //OnAttackCheck ��������Ʈ���� ȣ���� �Լ�.

	void JumpToTarget(float _JumpPower);
	
	UPROPERTY()
	UMyBossAnimInstance* BossAnim; //ĳ����Ŭ�������� �ִ� �ν��Ͻ��� ���� ����ϹǷ� ��������� �����Ѵ�.

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* DeadEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* JumpAttackEffect;

	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);

	static const float MaxHP;
	float CurrentHP;
	float AttackPower;//���� �Ŀ�
	float AttackRange;//���� ���� ���� ������ ���� (���ݱ���)
	float AttackRadius;//���� ���� ���� ������(���� ����)
	
	bool bIsAttackEnded;

	
	uint8 Phase = 1;

	void SetHP(const float& _NewHP);

	//UPROPERTY()
	//class AMyPlayer* CurrentAttacker; //���� �ֱٿ� Monster���� ���ظ� ���� Actor�� �����ϴ� ����.

	class AMyBossAIController* BossAIController;

};
