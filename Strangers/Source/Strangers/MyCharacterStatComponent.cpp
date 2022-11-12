// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyCharacterStatComponent::UMyCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;


	Level = 1;
	// ...
}


// Called when the game starts
void UMyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UMyCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == MyGameInstance) return;

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//������ ���� ĳ���� �����͸� �޾ƿ� CurrentStatData�� ����.
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP; 
	}
}

void UMyCharacterStatComponent::SetDamage(float NewDamage)
{
	if (nullptr != CurrentStatData)
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
		if (CurrentHP <= 0.0f)
		{
			OnHPIsZero.Broadcast(); //OnHPIsZero�� ���ε��Ǿ� �ִ� �Լ��� ȣ��.
		}
	}	
}

float UMyCharacterStatComponent::GetAttackPower()
{
	if (nullptr == CurrentStatData)
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
	return CurrentStatData->AttackPower;
}


