// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 인벤토리 슬롯 위젯블루프린트(UI) 에 연결된 베이스 클래스입니다.
 */
UCLASS()
class STRANGERS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemDataToUI(UTexture2D* MyIconTexture);
	bool Empty() const { return IsEmpty; };

protected:
	virtual void NativeConstruct() override; //위젯이 생성될 때 호출되는 함수.
	
private:
	struct FItemData* ItemData;

	int32 CurrentItemID = 0;

	UImage* ItemIcon; //UI_아이템 아이콘
	UTextBlock* NumberOfItems; //UI_현재 아이템 개수

	bool IsEmpty;
};
