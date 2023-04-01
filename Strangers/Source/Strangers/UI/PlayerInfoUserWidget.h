// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUserWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UMyPlayerStatComponent;

/**
 * 
 */
UCLASS()
class STRANGERS_API UPlayerInfoUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerStat(UMyPlayerStatComponent* NewPlayerStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateEXPWidget();
	void UpdateLevelWidget();

private:
	TWeakObjectPtr<UMyPlayerStatComponent> CurrentPlayerStat; //약포인터 사용

	UPROPERTY()
	UProgressBar* HPProgressBar;

	UPROPERTY()
	UProgressBar* EXPProgressBar;

	UPROPERTY()
	UTextBlock* LevelTextBlock;
	
};
