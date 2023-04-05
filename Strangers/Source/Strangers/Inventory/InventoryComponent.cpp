// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Inventory/Item_Interactable.h"
#include "UI/InventoryUserWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UInventoryComponent::AddItem(AItem_Interactable* Item)
{
	if (Capacity > Inventory.Num() && Item)
	{
		Inventory.Add(Item->GetItemData());
		OnInventoryUpdate.Broadcast();
		Item->Destroy();
	}
}

