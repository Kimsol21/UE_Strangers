// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindInventory(class UInventoryComponent* NewInventoryComponent);
	void UpdateInventoryUI();

private:
	TWeakObjectPtr<class UInventoryComponent> CurrentInventory; //�������� ���

	//UPROPERTY()
	//class UProgressBar* HPProgressBar;

};
