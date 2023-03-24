// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdateDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRANGERS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnInventoryUpdateDelegate OnInventoryUpdate;

	bool AddItem(class AItem_Interactable* Item); //�κ��丮�� ������ �߰�.
	void RemoveItem(); //�����ۻ����� �����Ҳ�.. 

	TArray<class AItem_Interactable*> Inventory; //�κ��丮

	int32 Capacity = 20; //�κ��丮 �뷮.


	

		
};
