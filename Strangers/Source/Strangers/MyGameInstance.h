// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) //플레이어 데이터 구조체
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public : 
	FMyCharacterData() : Level(1), MaxHP(100.0f), AttackPower(10.0f), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

USTRUCT(BlueprintType) //몬스터 데이터 구조체
struct FMyMonsterData : public FTableRowBase
{
	GENERATED_BODY()

public :
	FMyMonsterData() : ID(1), Name(TEXT("Corpse")), AttackPower(1.0f), MoveSpeed(300.0f), MaxHP(30.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

};


UCLASS()
class STRANGERS_API UMyGameInstance : public UGameInstance //게임 데이터 관리
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	FMyCharacterData* GetMyCharacterData(int32 Level);
	FMyMonsterData* GetMyMonsterData(int32 ID);

private:

	UPROPERTY()
	class UDataTable* MyCharacterTable;

	UPROPERTY()
	class UDataTable* MyMonsterTable;
};
