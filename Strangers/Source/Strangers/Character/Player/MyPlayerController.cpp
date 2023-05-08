// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/PlayerInfoUserWidget.h"
#include "Character/Player/MyPlayer.h"
#include "Inventory/Item_Interactable.h"
#include "UI/InventoryUserWidget.h"
#include "UI/InventorySlotWidget.h"
#include "UI/BossHPWidget.h"
#include "Character/Boss/MyBoss.h"
#include "Character/NPC/MyNPC.h"
#include "UI/DialogueWidget.h"
#include "UI/MyNoticeWidget.h"
#include "Kismet/GameplayStatics.h"

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

	static ConstructorHelpers::FClassFinder<UBossHPWidget> UI_BOSSHP(TEXT("WidgetBlueprint'/Game/UI/Boss/UI_BossHUD.UI_BossHUD_C'"));
	if (UI_BOSSHP.Succeeded())
	{
		BossHPClass = UI_BOSSHP.Class;
	}

	static ConstructorHelpers::FClassFinder<UDialogueWidget> UI_DIALOGUE(TEXT("WidgetBlueprint'/Game/UI/Dialogue/WG_Dialogue.WG_Dialogue_C'"));
	if (UI_DIALOGUE.Succeeded())
	{
		DialogueWidgetClass = UI_DIALOGUE.Class;
	}

	static ConstructorHelpers::FClassFinder<UMyNoticeWidget> UI_NOTICE(TEXT("WidgetBlueprint'/Game/UI/Notice/NoticePopup.NoticePopup_C'"));
	if (UI_NOTICE.Succeeded())
	{
		NoticeWidgetClass = UI_NOTICE.Class;
	}

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
	SetInputMode(InputGameOnly);

	possessedPawn = Cast<AMyPlayer>(GetPawn());
	if (nullptr == possessedPawn) return;

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

	//����HP ����
	BossHPWidget = CreateWidget<UBossHPWidget>(this, BossHPClass);
	BossHPWidget->AddToViewport();
	BossHPWidget->SetVisibility(ESlateVisibility::Collapsed);

	//��ȭâ ����
	DialogueWidget = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass);
	DialogueWidget->AddToViewport();
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);

	//Notice ����
	NoticeWidget = CreateWidget<UMyNoticeWidget>(this, NoticeWidgetClass);
	NoticeWidget->AddToViewport();
	NoticeWidget->SetVisibility(ESlateVisibility::Collapsed);
	NoticeWidget->BindUIToMyController(*this);
	
	
	//�÷��̾ NPC�� ������ �� ��������Ʈ.
	possessedPawn->OnPlayerMeetNPC().AddLambda([this](AMyNPC* _NPC)->void {
		AddPopup(*DialogueWidget);//UI�� �߰�.   
		//DialogueWidget->SetVisibility(ESlateVisibility::Visible);
		//_NPC->GetDialogueManagerComponent();
		DialogueWidget->BindUIToDialogueManager(_NPC->GetDialogueManagerComponent());
		});

	//��ȭ�� ������ �� ��������Ʈ.
	possessedPawn->OnDialogueFinished().AddLambda([this]()->void {
		RemoveCurrentPopup();
		});

	//�ó׸�ƽ ���� ���� ��������Ʈ.
	OnLevelSequenceStartDelegate.AddLambda([this]()->void {
		//UI �����.
		SetPlayerHUDVisibility(false);
		});

	//�ó׸�ƽ�� ������ ������ ���� ���� �� ��������Ʈ.
	OnBossRoomEnterDelegate.AddLambda([this](AMyBoss* _Boss)->void {
		SetPlayerHUDVisibility(true);
		_Boss->SetIsFighting(true); // �ش� ������ ������ On���ش�. 
		BossHPWidget->BindWidgetToBoss(_Boss); // ������ HPUI�� ����.
		BossHPWidget->SetVisibility(ESlateVisibility::Visible); // HPUI ���̰� �ϱ�.
		});

	//�˸�â�� ������Ʈ�� �� ��������Ʈ.
	OnNoticeUpdateDelegate.AddLambda([this](const FString& _NoticeString)->void {
		AddPopup(*NoticeWidget);
		});
	
}

////Pawn�� ���ǵǾ����� ȣ��Ǵ� �Լ�. 
//void AMyPlayerController::OnPossess(APawn* InPawn)
//{
//	possessedPawn = Cast<AMyPlayer>(InPawn); //���ǵ� �� ��������.
//	if (nullptr == possessedPawn) return;
//
//}

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
	InputComponent->BindAction(TEXT("DrinkPotion"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallDrinkPotion);
	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &AMyPlayerController::CallLockOn);

}

void AMyPlayerController::NextSentence()
{
	OnNextSentenceInputPressedEvent.Broadcast();
}

#pragma region InputBindFunctions

//void AMyPlayerController::PressedAnyKey()
//{
//	if (possessedPawn)
//	{
//		if (possessedPawn->GetIsPlayerTalking())
//		{
//			OnAnyInputPressedEvent.Broadcast();
//		}
//	}
//}



void AMyPlayerController::CallLockOn()
{
	if (possessedPawn)
	{
		possessedPawn->LockOn();
	}
}

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
	if (possessedPawn&&!possessedPawn->GetDoingSomething())
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

void AMyPlayerController::CallDrinkPotion()
{
	if (possessedPawn)
	{
		possessedPawn->DrinkPotion();
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
		possessedPawn->SetDoingSomething(true); // �÷��̾� �̵� ����.
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
			possessedPawn->SetDoingSomething(false); // �÷��̾� �̵� ���.
		}

		PopupWidgetArray.Top()->SetVisibility(ESlateVisibility::Collapsed); //���� �Ⱥ��̰� �ϱ�.
		PopupWidgetArray.Pop(); //�迭���� Ż����Ű��.
	}
}

void AMyPlayerController::SetPlayerHUDVisibility(bool _bVisible)
{
	if (_bVisible)
	{
		UserInfoWidget->SetVisibility(ESlateVisibility::Visible);
		SkillWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UserInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
		SkillWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}