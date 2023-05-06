// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractableInfoWidget.h"
#include "Components/TextBlock.h"

void UInteractableInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Text_InteractInfo = Cast<UTextBlock>(GetWidgetFromName("TEXT_InteractInfo"));
	if (nullptr == Text_InteractInfo) return;
}

void UInteractableInfoWidget::UpdateUI(FString _InteractInfo)
{
	Text_InteractInfo->SetText(FText::FromString(_InteractInfo));
}
