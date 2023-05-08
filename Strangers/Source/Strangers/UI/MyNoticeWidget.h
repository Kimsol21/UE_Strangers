// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyNoticeWidget.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UMyNoticeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindUIToMyController(class AMyPlayerController& _MyPlayerController);

protected:
	virtual void NativeConstruct() override; //AddToViewport �� ȣ��.

private:
	UPROPERTY()
	class UTextBlock* Text_Notice; //��ȭ ����.


};
