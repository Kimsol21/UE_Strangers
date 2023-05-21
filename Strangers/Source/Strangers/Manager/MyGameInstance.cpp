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

	FString MonsterDataPath = TEXT("DataTable'/Game/GameData/MyMonsterData.MyMonsterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MYMONSTER(*MonsterDataPath);
	if (DT_MYMONSTER.Succeeded())
	{
		MyMonsterTable = DT_MYMONSTER.Object;
	}


	FString ItemDataPath = TEXT("DataTable'/Game/GameData/ItemData.ItemData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item(*ItemDataPath);
	if (DT_Item.Succeeded())
	{
		ItemTable = DT_Item.Object;
	}

	
}

FMyCharacterData* UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT("")); //""은 에러메시지.
}

FMyMonsterData* UMyGameInstance::GetMyMonsterData(int32 ID)
{
	return MyMonsterTable->FindRow<FMyMonsterData>(*FString::FromInt(ID), TEXT(""));
}

FItemData* UMyGameInstance::GetItemData(int32 ID)
{
	return ItemTable->FindRow<FItemData>(*FString::FromInt(ID), TEXT(""));
}

//FDialogueSystemData* UMyGameInstance::GetDialogueData(uint8 Index)
//{
//	return DialogueTable->FindRow<FDialogueSystemData>(*FString::FromInt(Index), TEXT(""));
//}
