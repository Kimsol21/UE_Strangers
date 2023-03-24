// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonsterWidget.h"
#include "Character/Monster/MyMonsterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyMonsterWidget::BindMonsterStat(UMyMonsterStatComponent* NewMonsterStat) //MyCharacter�� BeginPlay()���� ȣ��.
{
	if (nullptr == NewMonsterStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NewMonsterStat is null."));
		return;
	}
	CurrentMonsterStat = NewMonsterStat;
	CurrentMonsterStat->OnHPChanged.AddUObject(this, &UMyMonsterWidget::UpdateHPWidget); //��������Ʈ�� �Լ� ���ε�.
}

void UMyMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//�̸��� ���� ProgressBar ���� �޾ƿ���.
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

	HPProgressBar->SetPercent(CurrentMonsterStat->GetHPRatio()); //UI�� HP�� ����.
}