// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoUserWidget.h"
#include "Character/Player/MyPlayerStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerInfoUserWidget::BindPlayerStat(UMyPlayerStatComponent* NewPlayerStat) //MyCharacter�� BeginPlay()���� ȣ��.
{
	if (nullptr == NewPlayerStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NewMonsterStat is null."));
		return;
	}
	CurrentPlayerStat = NewPlayerStat;
	CurrentPlayerStat->OnHPChanged.AddUObject(this, &UPlayerInfoUserWidget::UpdateHPWidget); //��������Ʈ�� �Լ� ���ε�.
	CurrentPlayerStat->OnEXPChanged.AddUObject(this, &UPlayerInfoUserWidget::UpdateEXPWidget); //��������Ʈ�� �Լ� ���ε�.
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

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	if (nullptr == HPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("HPProgressBar is null."));
		return;
	}

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
	EXPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_EXP")));
	if (nullptr == EXPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("EXPProgressBar is null."));
		return;
	}

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
	StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Stamina")));
	if (nullptr == StaminaProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("StaminaProgressBar is null."));
		return;
	}

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
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

	StaminaProgressBar->SetPercent(CurrentPlayerStat->GetStaminaRatio()); //UI�� ���׹̳ʰ� �ݿ�.
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

	HPProgressBar->SetPercent(CurrentPlayerStat->GetHPRatio()); //UI�� HP�� ����.
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

	EXPProgressBar->SetPercent(CurrentPlayerStat->GetEXPRatio()); //UI�� EXP�� ����.
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
