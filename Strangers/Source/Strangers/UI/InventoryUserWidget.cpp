// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUserWidget.h"
#include "Components/WrapBox.h"
#include "UI/InventorySlotWidget.h"
#include "Inventory/InventoryComponent.h"
#include "Character/Player/MyPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Inventory/Item_Interactable.h"

void UInventoryUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Controller 받아와 변수에 저장.
	CurrentController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentController)
	{
		//컨트롤러가 가지고 있는 슬롯위젯 받아오기.
		NewInventorySlotWidget = CurrentController->GetInventorySlotWidget();
	}

	//슬롯을 담을 변수를 UI 요소와 연결.
	InventorySlotMotherUI = Cast<UWrapBox>(GetWidgetFromName(TEXT("WrapBox_InventorySlotMother")));

	//인벤토리 슬롯 추가 로직. 추후 구현. 
	/*if (InventorySlotMotherUI)
	{
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
	}*/


	// 위젯트리에 있는 모든 인벤토리 슬롯을 찾아 Slots 배열에 저장합니다. 
	UWidgetTree* MyWidgetTree = Cast<UWidgetTree>(this->WidgetTree);
	TArray<UWidget*> SlotWidgets;
	MyWidgetTree->GetChildWidgets(InventorySlotMotherUI, SlotWidgets);

	for (UWidget* Widget : SlotWidgets)
	{
		//인벤토리 슬롯으로 캐스팅.
		UInventorySlotWidget* TempSlot = Cast<UInventorySlotWidget>(Widget);
		if (TempSlot)
		{
			Slots.Add(TempSlot); //인벤토리 슬롯 배열에 추가. 
		}
	}

}

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
	for (UInventorySlotWidget* s : Slots)
	{
		if (CurrentController)
		{
			
			if (s->Empty())
			{
				s->SetItemDataToUI(CurrentController->CurrentInteractable->GetIconTexture());
				break;
			}
		}
	}
}



