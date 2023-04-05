// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Interactable.generated.h"

UCLASS()
class STRANGERS_API AItem_Interactable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Interactable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ItemMesh;

	//플레이어가 this에 인터렉트 할 시에 호출되는 함수.
	void Interact(class AMyPlayerController* Controller);

	bool SetItemData();

	//GameInstance에서 불러올 ItemData 저장할 변수.
	
	UPROPERTY(EditAnywhere)
	int32 ID = 1;

	struct FItemData* ItemData;
	FItemData* GetItemData() const { return ItemData; };
	

	// ItemInfo 텍스트값 반환시켜주는 함수. UI블루프린트와 연동.
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	FString GetUseText() const; 

	UTexture2D* GetIconTexture() const { return IconTexture; };


private:
	UTexture2D* IconTexture; //아이콘텍스쳐
};
