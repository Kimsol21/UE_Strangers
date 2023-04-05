// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem_Interactable;

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdateDelegate); //�κ��丮�� ������Ʈ�ɶ� �˷��ִ� ��������Ʈ.

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

	void AddItem(AItem_Interactable* Item); //�κ��丮�� ������ �߰�.
	void RemoveItem(); //�����ۻ����� �����Ҳ�.. 

	TArray<AItem_Interactable*> GetInventory();

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UInventoryUserWidget* InventoryUserWidget;

	int32 Capacity = 20; //�κ��丮 �뷮.

private:

	TArray<struct FItemData*> Inventory; //�κ��丮


	

		
};
