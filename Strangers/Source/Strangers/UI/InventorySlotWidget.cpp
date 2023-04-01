// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIcon = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemIcon")));
	NumberOfItems = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_NumberOfItems")));
}

void UInventorySlotWidget::SetItemDataToUI(struct FItemData* ItemData)
{
	if (ItemData)
	{
		CurrentItemData = ItemData; //아이템 데이터 구조체 변수에 보관.

		if (ItemIcon)
		{
			//ItemIcon->SetBrushFromTexture()
		}

	}
}