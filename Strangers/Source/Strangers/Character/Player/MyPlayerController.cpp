// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/PlayerInfoUserWidget.h"
#include "Character/Player/MyPlayer.h"
#include "Inventory/Item_Interactable.h"
#include "UI/InventoryUserWidget.h"
#include "UI/InventorySlotWidget.h"

FInputModeGameAndUI InputGameAndUI; //둘다에 입력값 전달.
FInputModeUIOnly InputUIOnly; //UI에만 입력값 전달.
FInputModeGameOnly InputGameOnly;//게임에만 입력값 전달.

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

	possessedPawn = Cast<AMyPlayer>(GetPawn()); //빙의된 폰 가져오기.

	//플레이어 정보 위젯
	UserInfoWidget = CreateWidget<UPlayerInfoUserWidget>(this, UserInfoClass);
	UserInfoWidget->AddToViewport();

	UserInfoWidget->BindPlayerStat(possessedPawn->MyStat);

	//스킬위젯
	SkillWidget = CreateWidget<UUserWidget>(this, SkillClass);
	SkillWidget->AddToViewport();


	//아이템 정보 위젯
	ItemInfoWidget = CreateWidget<UUserWidget>(this, ItemInfoClass);
	ItemInfoWidget->AddToViewport();

	//인벤토리 슬롯 위젯
	InventorySlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

	//인벤토리 위젯
	InventoryUserWidget = CreateWidget<UInventoryUserWidget>(this, InventoryUserWidgetClass);
	InventoryUserWidget->BindInventory(possessedPawn->GetMyInventoryComponent());
	InventoryUserWidget->AddToViewport();
	InventoryUserWidget->SetVisibility(ESlateVisibility::Collapsed);

	

	
}

// Called to bind functionality to input
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 설정한 입력 세팅을 Pawn의 함수와 Binding시키기.
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
	if (InventoryUserWidget&&!InventoryUserWidget->IsVisible()) //InventoryUserWidget이 유효하고, 
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
	PopupWidgetArray.Add(&widget); //팝업 위젯 배열에 위젯 추가. 
	widget.SetVisibility(ESlateVisibility::Visible); //위젯 가시성 설정.

	if (1 == PopupWidgetArray.Num())//첫 팝업이면,
	{
		SetInputMode(InputUIOnly);//인풋모드 UI Only로 변경.
		SetShowMouseCursor(true); //마우스 커서 보여주기.
	}
}

void AMyPlayerController::RemoveCurrentPopup()
{
	if (0 == PopupWidgetArray.Num()) return;//팝업이 화면상에 아무것도 없으면 리턴.
	else
	{
		if (1 == PopupWidgetArray.Num())//마지막 팝업이면,
		{
			SetInputMode(InputGameOnly);//인풋모드 UI Only로 변경.
			SetShowMouseCursor(false); //마우스 커서 보여주기.
		}

		PopupWidgetArray.Top()->SetVisibility(ESlateVisibility::Collapsed); //위젯 안보이게 하기.
		PopupWidgetArray.Pop(); //배열에서 탈락시키기.
	}
}