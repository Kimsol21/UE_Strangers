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
	CurrentInventory->OnInventoryUpdate.AddUObject(this, &UInventoryUserWidget::UpdateInventoryUI); //��������Ʈ�� �Լ� ���ε�.
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
	

	//������ ���� ������ UI ��ҿ� ����.
	InventorySlotMotherUI = Cast<UWrapBox>(GetWidgetFromName(TEXT("WrapBox_InventorySlotMother"))); 
	if (InventorySlotMotherUI)
	{
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
	}

}

