// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Inventory/Item_Interactable.h"
#include "UI/InventoryUserWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//인벤토리 위젯 받아오기.
	/*UInventoryUserWidget InventoryWidget = Cast<UInventoryUserWidget>(InventoryWidget->GetUserWidgetObject());
	if (nullptr == InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget  is null!"));
		return;
	}
	InventoryWidget->BindMonsterStat(this);*/
	
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

