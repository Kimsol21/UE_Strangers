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

	//�÷��̾ this�� ���ͷ�Ʈ �� �ÿ� ȣ��Ǵ� �Լ�.
	void Interact(class AMyPlayerController* Controller);

	bool SetItemData();

	//GameInstance���� �ҷ��� ItemData ������ ����.
	
	UPROPERTY(EditAnywhere)
	int32 ID = 1;

	struct FItemData* ItemData;
	FItemData* GetItemData() const { return ItemData; };
	

	// ItemInfo �ؽ�Ʈ�� ��ȯ�����ִ� �Լ�. UI�������Ʈ�� ����.
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	FString GetUseText() const; 

	UTexture2D* GetIconTexture() const { return IconTexture; };


private:
	UTexture2D* IconTexture; //�������ؽ���
};
