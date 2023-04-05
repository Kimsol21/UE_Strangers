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

	//Controller �޾ƿ� ������ ����.
	CurrentController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentController)
	{
		//��Ʈ�ѷ��� ������ �ִ� �������� �޾ƿ���.
		NewInventorySlotWidget = CurrentController->GetInventorySlotWidget();
	}

	//������ ���� ������ UI ��ҿ� ����.
	InventorySlotMotherUI = Cast<UWrapBox>(GetWidgetFromName(TEXT("WrapBox_InventorySlotMother")));

	//�κ��丮 ���� �߰� ����. ���� ����. 
	/*if (InventorySlotMotherUI)
	{
		InventorySlotMotherUI->AddChildToWrapBox(NewInventorySlotWidget);
	}*/


	// ����Ʈ���� �ִ� ��� �κ��丮 ������ ã�� Slots �迭�� �����մϴ�. 
	UWidgetTree* MyWidgetTree = Cast<UWidgetTree>(this->WidgetTree);
	TArray<UWidget*> SlotWidgets;
	MyWidgetTree->GetChildWidgets(InventorySlotMotherUI, SlotWidgets);

	for (UWidget* Widget : SlotWidgets)
	{
		//�κ��丮 �������� ĳ����.
		UInventorySlotWidget* TempSlot = Cast<UInventorySlotWidget>(Widget);
		if (TempSlot)
		{
			Slots.Add(TempSlot); //�κ��丮 ���� �迭�� �߰�. 
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
	CurrentInventory->OnInventoryUpdate.AddUObject(this, &UInventoryUserWidget::UpdateInventoryUI); //��������Ʈ�� �Լ� ���ε�.
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



