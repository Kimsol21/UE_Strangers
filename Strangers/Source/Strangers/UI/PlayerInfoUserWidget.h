// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UPlayerInfoUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerStat(class UMyPlayerStatComponent* NewPlayerStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
	void UpdateEXPWidget();
	void UpdateLevelWidget();

private:
	TWeakObjectPtr<class UMyPlayerStatComponent> CurrentPlayerStat; //약포인터 사용

	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UProgressBar* EXPProgressBar;

	UPROPERTY()
	class UTextBlock* LevelTextBlock;
	
};
