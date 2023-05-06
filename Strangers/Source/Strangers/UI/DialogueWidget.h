// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UDialogueManagerComponent;

UCLASS()
class STRANGERS_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindUIToDialogueManager(UDialogueManagerComponent* _DialogueManager);
protected:
	virtual void NativeConstruct() override; //AddToViewport �� ȣ��.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UPROPERTY()
	UDialogueManagerComponent* CurrentDialogueManagerComponent;

	//void UpdateUI(FString _Speaker, FString _Conversation); //UI�� ������Ʈ �����ִ� �Լ�.

	UPROPERTY()
	UTextBlock* Text_Speaker; //���ϴ� ���.

	UPROPERTY()
	UTextBlock* Text_Conversation; //��ȭ ����.
};
