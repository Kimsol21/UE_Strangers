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
	DialogueTable->GetAllRows("Get All Dialogue Data", DialogueSheet); //데이터 테이블을 구조체 배열로 옮겨오기. 
	
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
	//호출 시점 : 플레이어와 NPC가 만났을 때 호출.
	//함수 내용 : 무슨 대화를 출력할지 판단. 
	//if(CurrentDialogueSheet.Empty())
	
	//배열이 비어있으면, 채워주기.
	if (0 == CurrentDialogueSheet.Num())
	{
		SetCurrentDialogueSheet(TEXT("Quest1"));
	}
		
	
	NextSentence();
	

}

void UDialogueManagerComponent::NextSentence()
{
	//호출 시점 : 대화중인 상태에서 플레이어 인풋이 들어왔을 때 호출.
	//함수 내용 : 대화를 다음 내용으로 넘겨줌.

	//대화 출력.
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
	//호출 시점 : 대화가 모두 끝났을 때 호출.
	//함수 내용 : 대화를 끝냄. 

	CurrentSentenceIndex = 0;
	CurrentDialogueSheet.Empty();

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyPlayerController->GetPossessedPawn()->OnDialogueFinished().Broadcast();

}


