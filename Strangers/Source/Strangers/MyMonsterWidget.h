// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API UMyMonsterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMyMonsterStatComponent* NewMonsterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UMyMonsterStatComponent> CurrentMonsterStat; //약포인터 사용

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
