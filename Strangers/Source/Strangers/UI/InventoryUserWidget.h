// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUserWidget.generated.h"

class UInventoryComponent;
class UInventorySlotWidget;
class UWrapBox;
class AMyPlayerController;

/**
 * 
 */   
UCLASS()
class STRANGERS_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()



public:
	void BindInventory(UInventoryComponent* const NewInventoryComponent);
	void UpdateInventoryUI();
  
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<UInventoryComponent> CurrentInventory; //약포인터 사용

	UInventorySlotWidget* NewInventorySlotWidget; //추가할 인벤토리 슬롯.

	UWrapBox* InventorySlotMotherUI; //실제 슬롯들을 담고 있는 WrapBox.

	TArray<UInventorySlotWidget*> Slots; //인벤토리 슬롯 배열. 

	AMyPlayerController* CurrentController; //플레이어 컨트롤러.

};
  