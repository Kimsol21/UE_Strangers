// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerStatComponent.h"
#include "Manager/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Player/MyPlayer.h"

// Sets default values for this component's properties
UMyPlayerStatComponent::UMyPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;


	Level = 1;

	bCanStaminaRecharge = true;
	// ...
}

void UMyPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UMyPlayerStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
#pragma region LOGIC About Stamina
	if (OwnerPlayer->IsSprinting()) //�������� ���̶��
	{
		//���׹̳� õõ�� ����. (����)
		SetStamina(FMath::FInterpConstantTo(CurrentStamina, 0.0f, DeltaTime, 15.0f));//FInterpConstantTo : ������ step���� Current���� Target���� float ����.
		if (CurrentStamina < KINDA_SMALL_NUMBER)
		{
			OwnerPlayer->SprintEnd();
		}
	}
	else
	{
		if (bCanStaminaRecharge && CurrentStamina < CurrentStatData->MaxStamina) //������ ������ �����̰�,���׹̳��� �ִ�ġ�� �ƴ϶��,
		{
			SetStamina(FMath::FInterpConstantTo(CurrentStamina, CurrentStatData->MaxStamina, DeltaTime, CurrentStatData->MaxStamina*0.25));// õõ�� ȸ��.
		}
	}
#pragma endregion 
}

// Called when the game starts
void UMyPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<AMyPlayer>(GetOwner());
	if (!OwnerPlayer) return;

	OwnerPlayer->OnStartDrinkPotion.AddLambda([this]()->void {
		SetHP(FMath::Clamp(CurrentHP + (CurrentStatData->MaxHP / 2), 0.0f, CurrentStatData->MaxHP));
		});
	// ...

	
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
		SetStamina(CurrentStatData->MaxStamina);
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

void UMyPlayerStatComponent::UseStamina(float _NewStamina)
{
	if (!CurrentStatData) return;

	SetStamina(CurrentStamina - _NewStamina);
}

void UMyPlayerStatComponent::SetStamina(float _NewStamina)
{
	if (!CurrentStatData) return;

	CurrentStamina = FMath::Clamp(_NewStamina, 0.0f, CurrentStatData->MaxStamina);

	OnStaminaChangedDelegate.Broadcast();

}

float UMyPlayerStatComponent::GetStaminaRatio()
{
	if (!CurrentStatData) return 0.0f;

	return (CurrentStamina/CurrentStatData->MaxStamina);
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

bool UMyPlayerStatComponent::IsStaminaZero()
{
	return CurrentStamina < 10 ? true : false;
}

