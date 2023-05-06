// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableInfoWidget.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class STRANGERS_API UInteractableInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override; //AddToViewport �� ȣ��.

private:
	void UpdateUI(FString _InteractInfo); //UI�� ������Ʈ �����ִ� �Լ�.

	UPROPERTY()
	UTextBlock* Text_InteractInfo; //TEXT : �÷��̾ ��ȣ�ۿ��� ������Ʈ�� ����.


};
