// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class STRANGERS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemDataToUI(struct FItemData* ItemData);

protected:
	virtual void NativeConstruct() override;


private:
	struct FItemData* CurrentItemData;

	UImage* ItemIcon;
	UTextBlock* NumberOfItems;
};
