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
	if (OwnerPlayer->IsSprinting()) //전력질주 중이라면
	{
		//스테미나 천천히 감소. (보간)
		SetStamina(FMath::FInterpConstantTo(CurrentStamina, 0.0f, DeltaTime, 15.0f));//FInterpConstantTo : 일정한 step으로 Current에서 Target까지 float 보간.
		if (CurrentStamina < KINDA_SMALL_NUMBER)
		{
			OwnerPlayer->SprintEnd();
		}
	}
	else
	{
		if (bCanStaminaRecharge && CurrentStamina < CurrentStatData->MaxStamina) //재충전 가능한 상태이고,스테미나가 최대치가 아니라면,
		{
			SetStamina(FMath::FInterpConstantTo(CurrentStamina, CurrentStatData->MaxStamina, DeltaTime, CurrentStatData->MaxStamina*0.25));// 천천히 회복.
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

	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);//레벨에 따른 캐릭터 데이터를 받아와 CurrentStatData에 저장.
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		SetStamina(CurrentStatData->MaxStamina);
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

bool UMyPlayerStatComponent::IsStaminaZero()
{
	return CurrentStamina < 10 ? true : false;
}

