// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonsterWidget.h"
#include "Character/Monster/MyMonsterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyMonsterWidget::BindMonsterStat(UMyMonsterStatComponent* NewMonsterStat) //MyCharacter의 BeginPlay()에서 호출.
{
	if (nullptr == NewMonsterStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NewMonsterStat is null."));
		return;
	}
	CurrentMonsterStat = NewMonsterStat;
	CurrentMonsterStat->OnHPChanged.AddUObject(this, &UMyMonsterWidget::UpdateHPWidget); //델리게이트에 함수 바인딩.
}

void UMyMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름을 통해 ProgressBar 위젯 받아오기.
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	if (nullptr == HPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("HPProgressBar is null."));
		return;
	}

	UpdateHPWidget();
}

void UMyMonsterWidget::UpdateHPWidget()
{
	if (!CurrentMonsterStat.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentMonsterStat is null."));
		return;
	}

	if (nullptr == HPProgressBar)
	{
		UE_LOG(LogTemp, Error, TEXT("HPProgressBar is null."));
		return;
	}

	HPProgressBar->SetPercent(CurrentMonsterStat->GetHPRatio()); //UI와 HP값 연동.
}