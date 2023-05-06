// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Manager/DialogueManagerComponent.h"


void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//�̸��� ���� ������ UI���� ��� �޾ƿ���.
	Text_Speaker = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Speaker1")));
	if (nullptr == Text_Speaker) return;

	Text_Conversation = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Conversation1")));
	if (nullptr == Text_Conversation) return;

	
	
}

void UDialogueWidget::BindUIToDialogueManager(UDialogueManagerComponent* _DialogueManager)
{
	if (_DialogueManager)
	{
		CurrentDialogueManagerComponent = _DialogueManager;
		//CurrentDialogueManagerComponent->OnUpdateDialogue().AddUObject(this, &UDialogueWidget::UpdateUI);
		CurrentDialogueManagerComponent->OnUpdateDialogue().AddLambda([this](FString _Speaker, FString _Conversation)->void {
			Text_Speaker->SetText(FText::FromString(_Speaker));
			Text_Conversation->SetText(FText::FromString(_Conversation));
			});
	}
	
}

void UDialogueWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}
//
//void UDialogueWidget::UpdateUI(FString _Speaker, FString _Conversation)
//{
//	//���ڷ� ���� ���� UI�� ������Ʈ �����ش�. 
//	Text_Speaker->SetText(FText::FromString(_Speaker)); 
//	Text_Conversation->SetText(FText::FromString(_Conversation));
//}
