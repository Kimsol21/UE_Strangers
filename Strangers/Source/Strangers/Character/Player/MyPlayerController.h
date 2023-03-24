// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayer.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STRANGERS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	AMyPlayer* possessedPawn;

	void CallUpDown(float NewAxisValue);
	void CallLeftRight(float NewAxisValue);
	void CallLookUp(float NewAxisValue);
	void CallTurn(float NewAxisValue);
	void CallZoomIn();
	void CallZoomOut();
	void CallJump();
	void CallAttack();
	void Interact();


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UPlayerInfoUserWidget> UserInfoClass;
	class UPlayerInfoUserWidget* UserInfoWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> SkillClass;
	class UUserWidget* SkillWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> ItemInfoClass;
	class UUserWidget* ItemInfoWidget;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	class AItem_Interactable* CurrentInteractable;


};
