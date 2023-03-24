// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyPlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate); //��������Ʈ ��ũ�� ����
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEXPChangedDelegate);
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

public:
	void SetNewLevel(int32 NewLevel); //������ �� �Լ��� ���ؼ��� �������� �� �� �ְ� �Ѵ�.

	void SetHP(float NewHP);
	void SetEXP(float NewEXP);
	void SetDamage(float NewDamage); //Character�� TakeDamage�Լ����� ȣ��.

	float GetAttackPower();
	float GetHPRatio();
	float GetEXPRatio();
	int32 GetLevel();

	FOnHpIsZeroDelegate OnHPIsZero; //��������Ʈ ����
	FOnHpChangedDelegate OnHPChanged;
	FOnEXPChangedDelegate OnEXPChanged;
	FOnLevelUpDelegate OnLevelUp;

private:
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) 
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentEXP;
		
};
