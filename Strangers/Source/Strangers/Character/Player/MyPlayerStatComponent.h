// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyPlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate); //��������Ʈ ��ũ�� ����
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEXPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStaminaChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelUpDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRANGERS_API UMyPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetNewLevel(int32 NewLevel); //������ �� �Լ��� ���ؼ��� �������� �� �� �ְ� �Ѵ�.
	void SetHP(float NewHP);
	void SetEXP(float NewEXP);
	void SetDamage(float NewDamage); //Character�� TakeDamage�Լ����� ȣ��.
	void SetStamina(float _NewStamina);
	void SetCanStaminaRecharge(bool _CanRecharge) { bCanStaminaRecharge = _CanRecharge; };

	void UseStamina(float _NewStamina);

	float GetAttackPower();
	float GetHPRatio();
	float GetEXPRatio();
	float GetStaminaRatio();
	int32 GetLevel();

	bool IsStaminaZero();

	FOnHpIsZeroDelegate OnHPIsZero; //��������Ʈ ����
	FOnHpChangedDelegate OnHPChanged;
	FOnEXPChangedDelegate OnEXPChanged;
	FOnLevelUpDelegate OnLevelUp;
	
	FOnStaminaChangedDelegate& OnStaminaChanged() { return OnStaminaChangedDelegate; };
private:
	UPROPERTY()
	class AMyPlayer* OwnerPlayer;

	bool bCanStaminaRecharge;

	FOnStaminaChangedDelegate OnStaminaChangedDelegate;
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) 
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentEXP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) // Transient  : ����ȭ? ó���� �ʱ�ȭ ���� ������ �����ϴµ�. ���߿� ��������.
	float CurrentStamina;
		
};
