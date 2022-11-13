// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include "MyCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UMyCharacterWidget::BindCharacterStat(UMyCharacterStatComponent* NewCharacterStat) //MyCharacter�� BeginPlay()���� ȣ��.
{
	if (nullptr == NewCharacterStat) return;

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyCharacterWidget::UpdateHPWidget); //��������Ʈ�� �Լ� ���ε�.
}

void UMyCharacterWidget::NativeConstruct()
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

void UMyCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio()); //UI�� HP�� ����.
		}
	}
}