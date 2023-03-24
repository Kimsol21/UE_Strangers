// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Inventory/Item_Interactable.h"

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

	// ...
	
}

bool UInventoryComponent::AddItem(AItem_Interactable* Item)
{
	if (Capacity >Inventory.Num() && Item)
	{
		Inventory.Add(Item);
		OnInventoryUpdate.Broadcast();
		Item->Destroy();
		return true;
	}
	return false;
}

//
// Called every frame
//void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	 ...
//}

