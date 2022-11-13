// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate); //��������Ʈ ��ũ�� ����
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRANGERS_API UMyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 NewLevel); //������ �� �Լ��� ���ؼ��� �������� �� �� �ְ� �Ѵ�.
	void SetHP(float NewHP);
	void SetDamage(float NewDamage); //Character�� TakeDamage�Լ����� ȣ��.

	float GetAttackPower();
	float GetHPRatio();

	FOnHpIsZeroDelegate OnHPIsZero; //��������Ʈ ����
	FOnHpChangedDelegate OnHPChanged;

private:
	struct FMyCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true)) 
	float CurrentHP;
		
};
