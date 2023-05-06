// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayer.h"
#include "MyPlayerController.generated.h"

//���漱��
class UUserWidget;
class UPlayerInfoUserWidget;
class UInventoryUserWidget;
class UInventorySlotWidget;
class AItem_Interactable;
class UBossHPWidget;
class AMyBoss;
class UDialogueWidget;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossRoomEnterDelegate, AMyBoss*);
DECLARE_EVENT(AMyPlayerController, FOnNextSentenceInputPressedEvent);

UCLASS()
class STRANGERS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

public:
	AMyPlayer* GetPossessedPawn() const { return possessedPawn; };
	UInventorySlotWidget* GetInventorySlotWidget() const { return InventorySlotWidget; };

	void SetCurrentInteractableItem(AItem_Interactable* Item) { CurrentInteractable = Item; };

	UFUNCTION(Blueprintcallable)
	void RemoveCurrentPopup(); //���� �ֱٿ� ����� �˾��� �����մϴ�.
	
	UFUNCTION(Blueprintcallable)
	void NextSentence();

	//������ ����	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	AItem_Interactable* CurrentInteractable;


	//�̺�Ʈ Get�Լ�.
	FOnBossRoomEnterDelegate& OnBossRoomEnter() { return OnBossRoomEnterDelegate; };
	FOnNextSentenceInputPressedEvent& OnNextSentenceInputPressed() { return OnNextSentenceInputPressedEvent; };

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	/*virtual void OnPossess(APawn* InPawn) override;*/
	
private:
	

	FOnNextSentenceInputPressedEvent OnNextSentenceInputPressedEvent;
	FOnBossRoomEnterDelegate OnBossRoomEnterDelegate; // �÷��̾ �ݸ����ȿ� ������ �� �̺�Ʈ.

	AMyPlayer* possessedPawn;

	//UI ����
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerInfoUserWidget> UserInfoClass;
	UPlayerInfoUserWidget* UserInfoWidget; //HUD:�÷��̾� ���� ����, �������.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UUserWidget> SkillClass;
	UUserWidget* SkillWidget; //HUD:��ų ���� ����, �����ϴ�.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UUserWidget> ItemInfoClass;
	UUserWidget* ItemInfoWidget; //HUD:������ ���� ����, ȭ���߾�.
	
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> BossHPClass;
	UBossHPWidget* BossHPWidget; //HUD:������ ���� ����, ȭ���߾�.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;
	UInventoryUserWidget* InventoryUserWidget; //Popup : �κ��丮â ����.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UInventorySlotWidget* InventorySlotWidget; //�߰��� �κ��丮 ����.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	UDialogueWidget* DialogueWidget; //��ȭ ����.

	TArray<UUserWidget*> PopupWidgetArray; //Popup �迭.

	////������ ����	
	//UPROPERTY(VisibleAnywhere, Category = Item)
	//AItem_Interactable* CurrentInteractable;

private:
	//��ǲ ���ε� �Լ���
	void CallUpDown(float NewAxisValue);
	void CallLeftRight(float NewAxisValue);
	void CallLookUp(float NewAxisValue);
	void CallTurn(float NewAxisValue);
	void CallZoomIn();
	void CallZoomOut();
	void CallJump();
	void CallAttack();
	void CallInteract();
	void CallInventory();
	void CallRoll();
	void CallDrinkPotion();
	void PressX();
	void CallLockOn();

	//UI ���� �Լ���
	void AddPopup(UUserWidget& widget); //���ڷ� ���� ������ ȭ�鿡 ���ϴ�.


};
