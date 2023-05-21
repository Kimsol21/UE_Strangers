// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSkillPanelWidget.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;

UCLASS()
class STRANGERS_API UPlayerSkillPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindPlayerSkillComponent(class UMyPlayerSkillComponent* _NewPlayerSkillComponent);

protected:
	virtual void NativeConstruct() override;

	void UpdateSkill_Cooltime_ProgressBar(uint8 _SkillID);
	void UpdateSkill_Cooltime_Text(uint8 _SkillID);

private:
	TWeakObjectPtr<class UMyPlayerSkillComponent> CurrentPlayerSkillComponent; //약포인터 사용

	UPROPERTY()
	UProgressBar* Skill1_Cooltime_ProgressBar;

	UPROPERTY()
	UProgressBar* Skill2_Cooltime_ProgressBar;

	UPROPERTY()
	UProgressBar* Skill3_Cooltime_ProgressBar;

	UPROPERTY()
	UTextBlock* Skill1_Cooltime_TextBlock;

	UPROPERTY()
	UTextBlock* Skill2_Cooltime_TextBlock;

	UPROPERTY()
	UTextBlock* Skill3_Cooltime_TextBlock;

};
