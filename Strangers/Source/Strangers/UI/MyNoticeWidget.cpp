// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyNoticeWidget.h"
#include "Character/Player/MyPlayerController.h"
#include "Components/TextBlock.h"

void UMyNoticeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름을 통해 변수와 UI위젯 요소 받아오기.
	Text_Notice = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Notice")));
	if (nullptr == Text_Notice) return;
}

void UMyNoticeWidget::BindUIToMyController(AMyPlayerController& _MyPlayerController)
{
	if (nullptr == &_MyPlayerController) return;

	_MyPlayerController.OnNoticeUpdate().AddLambda([this](const FString& _Speaker)->void {
		Text_Notice->SetText(FText::FromString(_Speaker));
		});

}
