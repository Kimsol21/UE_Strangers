// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayer.h"
#include "MyPlayerController.generated.h"

//전방선언
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
	void RemoveCurrentPopup(); //가장 최근에 띄워진 팝업을 삭제합니다.

	//아이템 관련	
	UPROPERTY(VisibleAnywhere, Category = Item)
		AItem_Interactable* CurrentInteractable;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	AMyPlayer* possessedPawn;

	//UI 관련
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerInfoUserWidget> UserInfoClass;
	UPlayerInfoUserWidget* UserInfoWidget; //HUD:플레이어 정보 위젯, 좌측상단.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UUserWidget> SkillClass;
	UUserWidget* SkillWidget; //HUD:스킬 정보 위젯, 우측하단.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UUserWidget> ItemInfoClass;
	UUserWidget* ItemInfoWidget; //HUD:아이템 정보 위젯, 화면중앙.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;
	UInventoryUserWidget* InventoryUserWidget; //Popup : 인벤토리창 위젯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UInventorySlotWidget* InventorySlotWidget; //추가할 인벤토리 슬롯.

	TArray<UUserWidget*> PopupWidgetArray; //Popup 배열.

	////아이템 관련	
	//UPROPERTY(VisibleAnywhere, Category = Item)
	//AItem_Interactable* CurrentInteractable;

private:
	//인풋 바인딩 함수들
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
	void PressX();

	//UI 관련 함수들
	void AddPopup(UUserWidget& widget); //인자로 들어온 위젯을 화면에 띄웁니다.

};
