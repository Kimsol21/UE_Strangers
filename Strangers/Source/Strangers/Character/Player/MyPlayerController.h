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
class UBossHPWidget;
class AMyBoss;
class UDialogueWidget;
class UPlayerSkillPanelWidget;

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossFightStartDelegate, AMyBoss*);
DECLARE_MULTICAST_DELEGATE(FOnBossFightEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelSequenceStartDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNoticeUpdateDelegate, const FString&);
DECLARE_EVENT(AMyPlayerController, FOnNextSentenceInputPressedEvent);
DECLARE_EVENT(AMyPlayerController, FOnExitKeyPressedEvent);

UCLASS()
class STRANGERS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerHUDVisibility(bool _bVisible);

	AMyPlayer* GetPossessedPawn() const { return possessedPawn; };
	UInventorySlotWidget* GetInventorySlotWidget() const { return InventorySlotWidget; };

	void SetCurrentInteractableItem(AItem_Interactable* Item) { CurrentInteractable = Item; };

	UFUNCTION(Blueprintcallable)
	void RemoveCurrentPopup(); //가장 최근에 띄워진 팝업을 삭제합니다.
	
	UFUNCTION(Blueprintcallable)
	void NextSentence();

	//아이템 관련	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	AItem_Interactable* CurrentInteractable;

	//이벤트 Get함수.
	FOnBossFightStartDelegate& OnBossFightStart() { return OnBossFightStartDelegate; };
	FOnNextSentenceInputPressedEvent& OnNextSentenceInputPressed() { return OnNextSentenceInputPressedEvent; };
	FOnNoticeUpdateDelegate& OnNoticeUpdate() { return OnNoticeUpdateDelegate; };
	FOnLevelSequenceStartDelegate& OnLevelSequenceStart() { return OnLevelSequenceStartDelegate; };
	FOnExitKeyPressedEvent& OnExitKeyPressed() { return OnExitKeyPressedEvent; };
	FOnBossFightEndDelegate& OnBossFightEnd() { return OnBossFightEndDelegate; };
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
private:
	UPROPERTY()
	class ATriggerBox* EndingTrigger;

	UPROPERTY()
	class AMyNPC* EndingNPC;

	//시네마틱 스킵 관련 변수.
	bool IsCinematicPlaying;


	FOnBossFightEndDelegate OnBossFightEndDelegate;
	FOnExitKeyPressedEvent OnExitKeyPressedEvent;
	FOnNextSentenceInputPressedEvent OnNextSentenceInputPressedEvent; //대화 중 다음 대화출력 인풋이 들어왔을 때 이벤트.
	FOnBossFightStartDelegate OnBossFightStartDelegate; // 플레이어가 콜리전안에 들어왔을 때 이벤트.
	FOnNoticeUpdateDelegate OnNoticeUpdateDelegate; // 알림 UI가 업데이트될 때 델리게이트.
	FOnLevelSequenceStartDelegate OnLevelSequenceStartDelegate; // 시네마틱 시작 직전 델리게이트.

	AMyPlayer* possessedPawn;

	//UI 관련
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerInfoUserWidget> UserInfoClass;
	UPlayerInfoUserWidget* UserInfoWidget; //HUD:플레이어 정보 위젯, 좌측상단.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UPlayerSkillPanelWidget> SkillClass;
	UPlayerSkillPanelWidget* SkillWidget; //HUD:스킬 정보 위젯, 우측하단.

	UPROPERTY(EditDefaultsOnly,  Category = UI)
	TSubclassOf<UUserWidget> ItemInfoClass;
	UUserWidget* ItemInfoWidget; //HUD:아이템 정보 위젯, 화면중앙.
	
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> BossHPClass;
	UBossHPWidget* BossHPWidget; //HUD:아이템 정보 위젯, 화면중앙.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventoryUserWidget> InventoryUserWidgetClass;
	UInventoryUserWidget* InventoryUserWidget; //Popup : 인벤토리창 위젯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;
	UInventorySlotWidget* InventorySlotWidget; //추가할 인벤토리 슬롯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	UDialogueWidget* DialogueWidget; //대화 위젯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UMyNoticeWidget> NoticeWidgetClass;
	UMyNoticeWidget* NoticeWidget; // 알림창 위젯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget> YouDiedWidgetClass;
	UUserWidget* YouDiedWidget; // YouDied 위젯.

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UUserWidget> BossFelledWidgetClass;
	UUserWidget* BossFelledWidget; // 보스 격파 위젯.

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
	void CallDrinkPotion();
	void PressX();
	void CallLockOn();
	void CallSkill_1();
	void CallSprint();
	void CallSprintEnd();

	//UI 관련 함수들
	void AddPopup(UUserWidget& widget); //인자로 들어온 위젯을 화면에 띄웁니다.


};
