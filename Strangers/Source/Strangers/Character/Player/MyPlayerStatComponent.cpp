// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerStatComponent.h"
#include "Manager/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyPlayerStatComponent::UMyPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;


	Level = 1;
	// ...
}


// Called when the game starts
void UMyPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UMyPlayerStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (nullptr == MyGameInstance) return;

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//레벨에 따른 캐릭터 데이터를 받아와 CurrentStatData에 저장.
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
}

void UMyPlayerStatComponent::SetHP(float NewHP)
{
	//좋은 코드구조bb : Stat중 HP를 변수로만 두고 변수의 값을 변경시켜준다면 만약 HP 변경시마다 뭔가를 해줘야 할 때 어디서 변경되었는지, 변경될때마다 처리를 해줘야하기도 하다.
	//하지만 이렇게 HP를 이 함수를 통해서만 변경할 수 있도록 한다면 매우 간단히 해결되는 문제이다. 

	CurrentHP = NewHP;
	OnHPChanged.Broadcast();//HP의 값이 변경되었음을 바인딩 된 함수들에게 알림.

	if (CurrentHP < KINDA_SMALL_NUMBER) //무시 가능한 오차를 측정할 때 사용하도록 언리얼이 제공하는 매크로
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();//HP가 Zero임을 바인딩 된 함수들에게 알림.
	}
}

void UMyPlayerStatComponent::SetEXP(float NewEXP)
{
	if (nullptr == CurrentStatData) UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));

	CurrentEXP += NewEXP;
	if (CurrentStatData->NextExp - CurrentEXP <= 0)
	{
		CurrentEXP = 0.0f;
		SetNewLevel(CurrentStatData->Level + 1); //레벨업!
		OnLevelUp.Broadcast();

	}
	OnEXPChanged.Broadcast(); //위젯과 연동 업데이트

	
}

void UMyPlayerStatComponent::SetDamage(float NewDamage)
{
	if (nullptr != CurrentStatData)
	{
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	}	
}

float UMyPlayerStatComponent::GetAttackPower()
{
	if (nullptr == CurrentStatData) UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		
	return CurrentStatData->AttackPower;
}

float UMyPlayerStatComponent::GetHPRatio()
{
	if(nullptr == CurrentStatData) UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));

	//책코드가 살짝 이상한것 같음. 질문필요.
	//return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
	return (CurrentHP / CurrentStatData->MaxHP);
}

float UMyPlayerStatComponent::GetEXPRatio()
{
	if (nullptr == CurrentStatData)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));
		return 0.0f;
	}
	return (CurrentEXP / CurrentStatData->NextExp);
}

int32 UMyPlayerStatComponent::GetLevel()
{
	if (nullptr == CurrentStatData) UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));

	return CurrentStatData->Level;
}

