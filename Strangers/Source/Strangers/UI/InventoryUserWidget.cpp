// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUserWidget.h"
#include "Components/WrapBox.h"
#include "UI/InventorySlotWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Character/Player/MyPlayerController.h"



void UInventoryUserWidget::BindInventory(UInventoryComponent* NewInventoryComponent)
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

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (MyController)
	{
		NewInventorySlotWidget = MyController->GetInventorySlotWidget();
	}
	

	//슬롯을 담을 변수를 UI 요소와 연결.
	InventorySlotMotherUI = Cast<UWrapBox>(GetWidgetFromName(TEXT("WrapBox_InventorySlotMother"))); 
	if (InventorySlotMotherUI)
	{
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
	}

}

