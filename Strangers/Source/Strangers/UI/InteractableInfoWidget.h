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
	virtual void NativeConstruct() override; //AddToViewport 시 호출.

private:
	void UpdateUI(FString _InteractInfo); //UI를 업데이트 시켜주는 함수.

	UPROPERTY()
	UTextBlock* Text_InteractInfo; //TEXT : 플레이어가 상호작용할 오브젝트의 정보.


};
