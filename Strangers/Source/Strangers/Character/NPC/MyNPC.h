// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Manager/DialogueManagerComponent.h"
#include "MyNPC.generated.h"


UCLASS()
class STRANGERS_API AMyNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	//콜리전 오버랩되었을 때 호출될 함수.
	UFUNCTION()
	void OnPlayerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UDialogueManagerComponent* GetDialogueManagerComponent() const { return DialogueManager; };

private:
	//FOnFoundPlayerEvent OnFoundPlayerEvent;

	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	class UDialogueManagerComponent* DialogueManager;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* PlayerOverlapCollision;



};
