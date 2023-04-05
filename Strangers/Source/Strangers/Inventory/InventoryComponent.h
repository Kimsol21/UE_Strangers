// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem_Interactable;

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdateDelegate); //인벤토리가 업데이트될때 알려주는 델리게이트.

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnInventoryUpdateDelegate OnInventoryUpdate;

	void AddItem(AItem_Interactable* Item); //인벤토리에 아이템 추가.
	void RemoveItem(); //아이템삭제는 어케할꼬.. 

	TArray<AItem_Interactable*> GetInventory();

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UInventoryUserWidget* InventoryUserWidget;

	int32 Capacity = 20; //인벤토리 용량.

private:

	TArray<struct FItemData*> Inventory; //인벤토리


	

		
};
