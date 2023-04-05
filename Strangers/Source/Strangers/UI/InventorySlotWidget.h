// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * �κ��丮 ���� �����������Ʈ(UI) �� ����� ���̽� Ŭ�����Դϴ�.
 */
UCLASS()
class STRANGERS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItemDataToUI(UTexture2D* MyIconTexture);
	bool Empty() const { return IsEmpty; };

protected:
	virtual void NativeConstruct() override; //������ ������ �� ȣ��Ǵ� �Լ�.
	
private:
	struct FItemData* ItemData;

	int32 CurrentItemID = 0;

	UImage* ItemIcon; //UI_������ ������
	UTextBlock* NumberOfItems; //UI_���� ������ ����

	bool IsEmpty;
};
