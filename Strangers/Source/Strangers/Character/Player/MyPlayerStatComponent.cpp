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

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//������ ���� ĳ���� �����͸� �޾ƿ� CurrentStatData�� ����.
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
}

void UMyPlayerStatComponent::SetHP(float NewHP)
{
	//���� �ڵ屸��bb : Stat�� HP�� �����θ� �ΰ� ������ ���� ��������شٸ� ���� HP ����ø��� ������ ����� �� �� ��� ����Ǿ�����, ����ɶ����� ó���� ������ϱ⵵ �ϴ�.
	//������ �̷��� HP�� �� �Լ��� ���ؼ��� ������ �� �ֵ��� �Ѵٸ� �ſ� ������ �ذ�Ǵ� �����̴�. 

	CurrentHP = NewHP;
	OnHPChanged.Broadcast();//HP�� ���� ����Ǿ����� ���ε� �� �Լ��鿡�� �˸�.

	if (CurrentHP < KINDA_SMALL_NUMBER) //���� ������ ������ ������ �� ����ϵ��� �𸮾��� �����ϴ� ��ũ��
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();//HP�� Zero���� ���ε� �� �Լ��鿡�� �˸�.
	}
}

void UMyPlayerStatComponent::SetEXP(float NewEXP)
{
	if (nullptr == CurrentStatData) UE_LOG(LogTemp, Error, TEXT("CurrentStatData is null."));

	CurrentEXP += NewEXP;
	if (CurrentStatData->NextExp - CurrentEXP <= 0)
	{
		CurrentEXP = 0.0f;
		SetNewLevel(CurrentStatData->Level + 1); //������!
		OnLevelUp.Broadcast();

	}
	OnEXPChanged.Broadcast(); //������ ���� ������Ʈ

	
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

	//å�ڵ尡 ��¦ �̻��Ѱ� ����. �����ʿ�.
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

