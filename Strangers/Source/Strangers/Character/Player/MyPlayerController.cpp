// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/PlayerInfoUserWidget.h"
#include "Character/Player/MyPlayer.h"
#include "Inventory/Item_Interactable.h"
#include "UI/InventoryUserWidget.h"
#include "UI/InventorySlotWidget.h"

FInputModeGameAndUI InputGameAndUI; //�Ѵٿ� �Է°� ����.
FInputModeUIOnly InputUIOnly; //UI���� �Է°� ����.
FInputModeGameOnly InputGameOnly;//���ӿ��� �Է°� ����.

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerInfoUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/UI_PlayerInfo.UI_PlayerInfo_C'"));
	if (UI_HUD.Succeeded())
	{
		UserInfoClass = UI_HUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_SKILL(TEXT("WidgetBlueprint'/Game/UI/UI_Skill.UI_Skill_C'"));
	if (UI_SKILL.Succeeded())
	{
		SkillClass = UI_SKILL.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_ITEM_INFO(TEXT("WidgetBlueprint'/Game/UI/Inventory/WG_ItemInfo.WG_ItemInfo_C'"));
	if (UI_ITEM_INFO.Succeeded())
	{
		ItemInfoClass = UI_ITEM_INFO.Class;
	}

	static ConstructorHelpers::FClassFinder<UInventorySlotWidget> UI_INVENTORY_SLOT(TEXT("WidgetBlueprint'/Game/UI/Inventory/WG_InvenSlot.WG_InvenSlot_C'"));
	if (UI_INVENTORY_SLOT.Succeeded())
	{
		InventorySlotWidgetClass = UI_INVENTORY_SLOT.Class;
	}

	static ConstructorHelpers::FClassFinder<UInventoryUserWidget> UI_INVENTORY(TEXT("WidgetBlueprint'/Game/UI/Inventory/WG_Inventory.WG_Inventory_C'"));
	if (UI_INVENTORY.Succeeded())
	{
		InventoryUserWidgetClass = UI_INVENTORY.Class;
	}

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
	SetInputMode(InputGameOnly);

	possessedPawn = Cast<AMyPlayer>(GetPawn()); //���ǵ� �� ��������.

	//�÷��̾� ���� ����
	UserInfoWidget = CreateWidget<UPlayerInfoUserWidget>(this, UserInfoClass);
	UserInfoWidget->AddToViewport();

	UserInfoWidget->BindPlayerStat(possessedPawn->MyStat);

	//��ų����
	SkillWidget = CreateWidget<UUserWidget>(this, SkillClass);
	SkillWidget->AddToViewport();


	//������ ���� ����
	ItemInfoWidget = CreateWidget<UUserWidget>(this, ItemInfoClass);
	ItemInfoWidget->AddToViewport();

	//�κ��丮 ���� ����
	InventorySlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

	//�κ��丮 ����
	InventoryUserWidget = CreateWidget<UInventoryUserWidget>(this, InventoryUserWidgetClass);
	InventoryUserWidget->BindInventory(possessedPawn->GetMyInventoryComponent());
	InventoryUserWidget->AddToViewport();
	InventoryUserWidget->SetVisibility(ESlateVisibility::Collapsed);

	

	
}

// Called to bind functionality to input
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// ������ �Է� ������ Pawn�� �Լ��� Binding��Ű��.
	InputComponent->BindAxis(TEXT("UpDown"), this, &AMyPlayerController::CallUpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPlayerController::CallLeftRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayerController::CallLookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &AMyPlayerController::CallTurn);
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallJump);
	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallAttack);
	InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallZoomIn);
	InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallZoomOut);
	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallInteract);
	InputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallInventory);
	InputComponent->BindAction(TEXT("Quit"), EInputEvent::IE_Pressed, this, &AMyPlayerController::PressX);
	InputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallRoll);

}

#pragma region InputBindFunctions

void AMyPlayerController::CallRoll()
{
	if (possessedPawn)
	{
		possessedPawn->Roll();
	}
}

void AMyPlayerController::CallInventory()
{
	if (InventoryUserWidget&&!InventoryUserWidget->IsVisible()) //InventoryUserWidget�� ��ȿ�ϰ�, 
	{
		AddPopup(*InventoryUserWidget);
	}
}

void AMyPlayerController::PressX()
{
	RemoveCurrentPopup();
}

void AMyPlayerController::CallInteract()
{  
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}

void AMyPlayerController::CallUpDown(float NewAxisValue)
{
	if (possessedPawn)
	{
		possessedPawn->UpDown(NewAxisValue);
	}
}

void AMyPlayerController::CallLeftRight(float NewAxisValue)
{
	if (possessedPawn)
	{
		possessedPawn->LeftRight(NewAxisValue);
	}
}

void AMyPlayerController::CallLookUp(float NewAxisValue)
{
	if (possessedPawn)
	{
		possessedPawn->LookUp(NewAxisValue);
	}
}

void AMyPlayerController::CallTurn(float NewAxisValue)
{
	if (possessedPawn)
	{
		possessedPawn->Turn(NewAxisValue);
	}
}

void AMyPlayerController::CallZoomIn()
{
	if (possessedPawn)
	{
		possessedPawn->ZoomIn();
	}
}

void AMyPlayerController::CallZoomOut()
{
	if (possessedPawn)
	{
		possessedPawn->ZoomOut();
	}
}

void AMyPlayerController::CallJump()
{
	if (possessedPawn)
	{
		possessedPawn->Jump();
	}
} 

void AMyPlayerController::CallAttack()
{ 
	if (possessedPawn)
	{
		possessedPawn->Attack();
	}
}

#pragma endregion


void AMyPlayerController::AddPopup(UUserWidget& widget)
{
	PopupWidgetArray.Add(&widget); //�˾� ���� �迭�� ���� �߰�. 
	widget.SetVisibility(ESlateVisibility::Visible); //���� ���ü� ����.

	if (1 == PopupWidgetArray.Num())//ù �˾��̸�,
	{
		SetInputMode(InputUIOnly);//��ǲ��� UI Only�� ����.
		SetShowMouseCursor(true); //���콺 Ŀ�� �����ֱ�.
	}
}

void AMyPlayerController::RemoveCurrentPopup()
{
	if (0 == PopupWidgetArray.Num()) return;//�˾��� ȭ��� �ƹ��͵� ������ ����.
	else
	{
		if (1 == PopupWidgetArray.Num())//������ �˾��̸�,
		{
			SetInputMode(InputGameOnly);//��ǲ��� UI Only�� ����.
			SetShowMouseCursor(false); //���콺 Ŀ�� �����ֱ�.
		}

		PopupWidgetArray.Top()->SetVisibility(ESlateVisibility::Collapsed); //���� �Ⱥ��̰� �ϱ�.
		PopupWidgetArray.Pop(); //�迭���� Ż����Ű��.
	}
}