// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DialogueManagerComponent.h"
#include "Manager/MyGameInstance.h"
#include "Character/Player/MyPlayerController.h"

// Sets default values for this component's properties
UDialogueManagerComponent::UDialogueManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	FString DialogueDataPath = TEXT("DataTable'/Game/GameData/Dialogue_JapanGirl.Dialogue_JapanGirl'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Dialogue(*DialogueDataPath);
	if (DT_Dialogue.Succeeded())
	{
		DialogueTable = DT_Dialogue.Object;
	}
	DialogueTable->GetAllRows("Get All Dialogue Data", DialogueSheet); //������ ���̺��� ����ü �迭�� �Űܿ���. 
	
	CurrentSentenceIndex = 0;
}


// Called when the game starts
void UDialogueManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->OnNextSentenceInputPressed().AddUObject(this, &UDialogueManagerComponent::NextSentence);
	// ...
	
}


// Called every frame
void UDialogueManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueManagerComponent::SetCurrentDialogueSheet(FString _Branch)
{
	for (FDialogueSystemData* Sentence : DialogueSheet)
	{
		if (_Branch == Sentence->Branch)
		{
			CurrentDialogueSheet.Add(Sentence);
		}
	}
}

void UDialogueManagerComponent::StartDialogue()
{
	//ȣ�� ���� : �÷��̾�� NPC�� ������ �� ȣ��.
	//�Լ� ���� : ���� ��ȭ�� ������� �Ǵ�. 
	//if(CurrentDialogueSheet.Empty())
	
	//�迭�� ���������, ä���ֱ�.
	if (0 == CurrentDialogueSheet.Num())
	{
		SetCurrentDialogueSheet(TEXT("Quest1"));
	}
		
	
	NextSentence();
	

}

void UDialogueManagerComponent::NextSentence()
{
	//ȣ�� ���� : ��ȭ���� ���¿��� �÷��̾� ��ǲ�� ������ �� ȣ��.
	//�Լ� ���� : ��ȭ�� ���� �������� �Ѱ���.

	//��ȭ ���.
	if (0 == CurrentDialogueSheet.Num()) return;

	if (CurrentDialogueSheet.Num()> CurrentSentenceIndex)
	{
		OnUpdateDialogueEvent.Broadcast(CurrentDialogueSheet[CurrentSentenceIndex]->Speaker, CurrentDialogueSheet[CurrentSentenceIndex]->Conversation);
		CurrentSentenceIndex++;
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueManagerComponent::EndDialogue()
{
	//ȣ�� ���� : ��ȭ�� ��� ������ �� ȣ��.
	//�Լ� ���� : ��ȭ�� ����. 

	CurrentSentenceIndex = 0;
	CurrentDialogueSheet.Empty();

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->GetPossessedPawn()->OnDialogueFinished().Broadcast();

}


