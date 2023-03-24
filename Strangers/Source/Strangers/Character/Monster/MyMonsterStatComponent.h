// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyMonsterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpIsZeroDelegate); //��������Ʈ ��ũ�� ����
DECLARE_MULTICAST_DELEGATE(FOnHpChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UMyMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyMonsterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetNewMonster(int32 ID); //����ID�� �� �Լ��� ���ؼ��� �������� �� �� �ְ� �Ѵ�.

	void SetHP(float NewHP); //HP����
	void SetDamage(float NewDamage); //MyMonster�� TakeDamage�Լ����� ȣ��.

	float GetAttackPower(); //���ݷ� ��������
	float GetEXPGive();//����ġ ��������
	float GetHPRatio(); //���� HP���� ��������
	float GetMoveSpeed(); //���� �̵��ӵ� ��������

	FOnHpIsZeroDelegate OnHPIsZero; //��������Ʈ ����
	FOnHpChangedDelegate OnHPChanged;

private:

	struct FMyMonsterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 ID;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

		
};
