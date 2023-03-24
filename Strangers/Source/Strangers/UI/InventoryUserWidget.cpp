// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUserWidget.h"
#include "Inventory/InventoryComponent.h"


void UInventoryUserWidget::BindInventory(class UInventoryComponent* NewInventoryComponent)
{
	if (nullptr == NewInventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryComponent is null."));
		return;
	}
	CurrentInventory = NewInventoryComponent;
	CurrentInventory->OnInventoryUpdate.AddUObject(this, &UInventoryUserWidget::UpdateInventoryUI); //델리게이트에 함수 바인딩.
}

void UInventoryUserWidget::UpdateInventoryUI()
{

}