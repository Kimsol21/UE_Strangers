// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/GameData/MyCharacterData.MyCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYCHARACTER(*CharacterDataPath);
	if (DT_MYCHARACTER.Succeeded())
	{
		MyCharacterTable = DT_MYCHARACTER.Object;
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}
