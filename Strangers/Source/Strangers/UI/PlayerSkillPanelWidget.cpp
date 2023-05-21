// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerSkillPanelWidget.h"
#include "Character/Player/MyPlayerSkillComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerSkillPanelWidget::BindPlayerSkillComponent(class UMyPlayerSkillComponent* _NewPlayerSkillComponent)
{
	if (!_NewPlayerSkillComponent) return;

	CurrentPlayerSkillComponent = _NewPlayerSkillComponent;
	//CurrentPlayerSkillComponent->OnSkillCooltimeValueChange().AddUObject(this, &UPlayerSkillPanelWidget::UpdateSkill_Cooltime_ProgressBar); //델리게이트에 함수 바인딩.
	//CurrentPlayerSkillComponent->OnSkillCooltimeValueChange().AddUObject(this, &UPlayerSkillPanelWidget::UpdateSkill_Cooltime_ProgressBar); //델리게이트에 함수 바인딩.
	CurrentPlayerSkillComponent->OnSkillCooltimeValueChange().AddLambda([this](uint8 _SkillID)->void{
		UpdateSkill_Cooltime_ProgressBar(_SkillID);
		UpdateSkill_Cooltime_Text(_SkillID);
		});

	UpdateSkill_Cooltime_ProgressBar(1);
	UpdateSkill_Cooltime_ProgressBar(2);
	UpdateSkill_Cooltime_ProgressBar(3);

	UpdateSkill_Cooltime_Text(1);
	UpdateSkill_Cooltime_Text(2);
	UpdateSkill_Cooltime_Text(3);

}

void UPlayerSkillPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름을 통해 ProgressBar 위젯 받아오기.
	Skill1_Cooltime_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PG_Skill1_Cooltime")));
	if (!Skill1_Cooltime_ProgressBar) return;

	Skill2_Cooltime_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PG_Skill2_Cooltime")));
	if (!Skill2_Cooltime_ProgressBar) return;

	Skill3_Cooltime_ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PG_Skill3_Cooltime")));
	if (!Skill3_Cooltime_ProgressBar) return;
	
	Skill1_Cooltime_TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Skill1_Cooltime")));
	if (!Skill1_Cooltime_TextBlock) return;

	Skill2_Cooltime_TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Skill2_Cooltime")));
	if (!Skill2_Cooltime_TextBlock) return;

	Skill3_Cooltime_TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Skill3_Cooltime")));
	if (!Skill3_Cooltime_TextBlock) return;
}



void UPlayerSkillPanelWidget::UpdateSkill_Cooltime_ProgressBar(uint8 _SkillID)
{
	if (!CurrentPlayerSkillComponent.IsValid()) return;

	switch (_SkillID)
	{
	case 1:
		Skill1_Cooltime_ProgressBar->SetPercent(CurrentPlayerSkillComponent->GetCooltimeRatio(_SkillID)); //UI와 값 연동.
		break;
	case 2:
		Skill2_Cooltime_ProgressBar->SetPercent(CurrentPlayerSkillComponent->GetCooltimeRatio(_SkillID)); //UI와 값 연동.
		break;
	case 3:
		Skill3_Cooltime_ProgressBar->SetPercent(CurrentPlayerSkillComponent->GetCooltimeRatio(_SkillID)); //UI와 값 연동.
		break;
	default:
		break;
	}
}

void UPlayerSkillPanelWidget::UpdateSkill_Cooltime_Text(uint8 _SkillID)
{
	if (!CurrentPlayerSkillComponent.IsValid()) return;

	switch (_SkillID)
	{
	case 1:
		Skill1_Cooltime_TextBlock->SetText(CurrentPlayerSkillComponent->GetCooltimeText(1));
		break;
	case 2:
		Skill2_Cooltime_TextBlock->SetText(CurrentPlayerSkillComponent->GetCooltimeText(2));
		break;
	case 3:
		Skill3_Cooltime_TextBlock->SetText(CurrentPlayerSkillComponent->GetCooltimeText(3));
		break;
	default:
		break;
	}
}

