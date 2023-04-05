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
	TWeakObjectPtr<UInventoryComponent> CurrentInventory; //�������� ���

	UInventorySlotWidget* NewInventorySlotWidget; //�߰��� �κ��丮 ����.

	UWrapBox* InventorySlotMotherUI; //���� ���Ե��� ��� �ִ� WrapBox.

	TArray<UInventorySlotWidget*> Slots; //�κ��丮 ���� �迭. 

	AMyPlayerController* CurrentController; //�÷��̾� ��Ʈ�ѷ�.

};
  