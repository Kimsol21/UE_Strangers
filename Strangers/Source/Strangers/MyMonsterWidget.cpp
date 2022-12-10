// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonsterWidget.h"
#include "MyMonsterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyMonsterWidget::BindCharacterStat(UMyMonsterStatComponent* NewMonsterStat) //MyCharacter�� BeginPlay()���� ȣ��.
{
	if (nullptr == NewMonsterStat) return;

	CurrentMonsterStat = NewMonsterStat;
	NewMonsterStat->OnHPChanged.AddUObject(this, &UMyMonsterWidget::UpdateHPWidget); //��������Ʈ�� �Լ� ���ε�.
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
	if (CurrentMonsterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentMonsterStat->GetHPRatio()); //UI�� HP�� ����.
		}
	}
}