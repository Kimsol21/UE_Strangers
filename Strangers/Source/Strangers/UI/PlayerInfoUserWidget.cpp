// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoUserWidget.h"
#include "Character/Player/MyPlayerStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerInfoUserWidget::BindPlayerStat(UMyPlayerStatComponent* NewPlayerStat) //MyCharacter의 BeginPlay()에서 호출.
{
	if (nullptr == NewPlayerStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NewMonsterStat is null."));
		return;
	}
	CurrentPlayerStat = NewPlayerStat;
	CurrentPlayerStat->OnHPChanged.AddUObject(this, &UPlayerInfoUserWidget::UpdateHPWidget); //델리게이트에 함수 바인딩.
	CurrentPlayerStat->OnEXPChanged.AddUObject(this, &UPlayerInfoUserWidget::UpdateEXPWidget); //델리게이트에 함수 바인딩.
	CurrentPlayerStat->OnLevelUp.AddUObject(this, &UPlayerInfoUserWidget::UpdateLevelWidget);
	CurrentPlayerStat->OnStaminaChanged().AddUObject(this, &UPlayerInfoUserWidget::UpdateStaminaWidget);
	UpdateHPWidget();
	UpdateEXPWidget();
	UpdateLevelWidget();
	UpdateStaminaWidget();
}

void UPlayerInfoUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름을 통해 ProgressBar 위젯 받아오기.
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	if (nullptr == HPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("HPProgressBar is null."));
		return;
	}

	//이름을 통해 ProgressBar 위젯 받아오기.
	EXPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_EXP")));
	if (nullptr == EXPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("EXPProgressBar is null."));
		return;
	}

	//이름을 통해 ProgressBar 위젯 받아오기.
	StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Stamina")));
	if (nullptr == StaminaProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("StaminaProgressBar is null."));
		return;
	}

	//이름을 통해 ProgressBar 위젯 받아오기.
	LevelTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Level")));
	if (nullptr == LevelTextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("LevelTextBlock is null."));
		return;
	}
}

void UPlayerInfoUserWidget::UpdateStaminaWidget()
{
	if (!CurrentPlayerStat.IsValid() || !HPProgressBar) return;

	StaminaProgressBar->SetPercent(CurrentPlayerStat->GetStaminaRatio()); //UI에 스테미너값 반영.
}

void UPlayerInfoUserWidget::UpdateHPWidget()
{
	if (!CurrentPlayerStat.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentPlayerStat is null"));
		return;
	}

	if (nullptr == HPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("HPProgressBar is null."));
		return;
	}

	HPProgressBar->SetPercent(CurrentPlayerStat->GetHPRatio()); //UI와 HP값 연동.
}

void UPlayerInfoUserWidget::UpdateEXPWidget()
{
	if (!CurrentPlayerStat.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentPlayerStat is null."));
		return;
	}

	if (nullptr == EXPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("EXPProgressBar is null."));
		return;
	}

	EXPProgressBar->SetPercent(CurrentPlayerStat->GetEXPRatio()); //UI와 EXP값 연동.
}

void UPlayerInfoUserWidget::UpdateLevelWidget()
{
	if (!CurrentPlayerStat.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentPlayerStat is null."));
		return;
	}

	if (nullptr == LevelTextBlock)
	{
		UE_LOG(LogTemp, Error, TEXT("EXPProgressBar is null."));
		return;
	}
	LevelTextBlock->SetText(FText::AsNumber(CurrentPlayerStat->GetLevel() ) );

}
