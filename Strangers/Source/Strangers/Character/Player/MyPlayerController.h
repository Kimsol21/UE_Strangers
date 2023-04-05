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

/**
 * 
 */
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

	//������ ����	
	UPROPERTY(VisibleAnywhere, Category = Item)
		AItem_Interactable* CurrentInteractable;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
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
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;
	UInventoryUserWidget* InventoryUserWidget; //Popup : �κ��丮â ����.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UInventorySlotWidget* InventorySlotWidget; //�߰��� �κ��丮 ����.

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
	void PressX();

	//UI ���� �Լ���
	void AddPopup(UUserWidget& widget); //���ڷ� ���� ������ ȭ�鿡 ���ϴ�.

};
