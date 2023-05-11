// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonsterStatComponent.h"
#include "Manager/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyMonsterStatComponent::UMyMonsterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	ID = 1;
	// ...
}


// Called when the game starts
void UMyMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyMonsterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewMonster(ID);
}

void UMyMonsterStatComponent::SetNewMonster(int32 _ID)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == MyGameInstance) return;

	CurrentStatData = MyGameInstance->GetMyMonsterData(_ID);//�ش� ID�� ���� Monster�� DataTable���� ã�� �ҷ��� �� CurrentStatData�� ����.
	if (nullptr != CurrentStatData)
	{
		ID = _ID;
		SetHPFull();
	}
}

void UMyMonsterStatComponent::SetHPFull()
{
	SetHP(CurrentStatData->MaxHP); 
}

void UMyMonsterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();//HP�� ���� ����Ǿ����� ���ε� �� �Լ��鿡�� �˸�.

	if (CurrentHP < KINDA_SMALL_NUMBER) //���� ������ ������ ������ �� ����ϵ��� �𸮾��� �����ϴ� ��ũ��
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();//HP�� Zero���� ���ε� �� �Լ��鿡�� �˸�.
	}
}

void UMyMonsterStatComponent::SetDamage(float NewDamage)
{
	if (nullptr != CurrentStatData)
	{
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	}
}

float UMyMonsterStatComponent::GetAttackPower()
{
	if (nullptr == CurrentStatData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		return 0.0f;
	}

	return CurrentStatData->AttackPower;
}

float UMyMonsterStatComponent::GetEXPGive()
{
	if (nullptr == CurrentStatData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		return 0.0f;
	}

	return CurrentStatData->EXPGive;
}

float UMyMonsterStatComponent::GetHPRatio()
{
	if (nullptr == CurrentStatData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		return 0.0f;
	}

	return (CurrentHP / CurrentStatData->MaxHP);
}

float UMyMonsterStatComponent::GetMoveSpeed()
{
	if (nullptr == CurrentStatData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		return 0.0f;
	}

	return CurrentStatData->MoveSpeed;
}

