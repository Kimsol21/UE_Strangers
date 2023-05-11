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

	CurrentStatData = MyGameInstance->GetMyMonsterData(_ID);//해당 ID를 가진 Monster를 DataTable에서 찾아 불러온 후 CurrentStatData에 저장.
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
	OnHPChanged.Broadcast();//HP의 값이 변경되었음을 바인딩 된 함수들에게 알림.

	if (CurrentHP < KINDA_SMALL_NUMBER) //무시 가능한 오차를 측정할 때 사용하도록 언리얼이 제공하는 매크로
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();//HP가 Zero임을 바인딩 된 함수들에게 알림.
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

