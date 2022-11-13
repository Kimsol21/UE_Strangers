// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UMyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMyCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UMyCharacterStatComponent> CurrentCharacterStat; //약포인터 사용

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
