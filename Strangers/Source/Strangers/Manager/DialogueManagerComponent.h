// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "DialogueManagerComponent.generated.h"




USTRUCT(BlueprintType) //��ȭ �ý��� ������ ����ü
struct FDialogueSystemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FDialogueSystemData() : Index(0), Type("Normal"), Branch("Default"), Speaker("SpeakerName"), Conversation("This is Default Conversation.") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		uint8 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Branch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Conversation;

};




DECLARE_EVENT_TwoParams(UDialogueManagerComponent, FOnUpdateDialogueEvent, FString, FString);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UDialogueManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartDialogue();
	void NextSentence();
	void EndDialogue();

	FOnUpdateDialogueEvent& OnUpdateDialogue() { return OnUpdateDialogueEvent;};

	UPROPERTY(EditAnywhere, Category = "Dialogue") // �뺻
	class UDataTable* DialogueTable;

private:
	void SetCurrentDialogueSheet(FString _Branch);

	FOnUpdateDialogueEvent OnUpdateDialogueEvent;

	TArray<FDialogueSystemData*> DialogueSheet; // ��ü ������ ���̺�.
	TArray<FDialogueSystemData*> CurrentDialogueSheet; // ������ ����� ��ȭ ��Ʈ.
	uint8 CurrentSentenceIndex;
	

};
