// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIcon = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemIcon")));
	NumberOfItems = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_NumberOfItems")));
	IsEmpty = true;
}

void UInventorySlotWidget::SetItemDataToUI(UTexture2D* MyIconTexture)
{
	FSlateBrush b;
	b.SetResourceObject(MyIconTexture);
	ItemIcon->SetBrush(b);
	IsEmpty = false;
}