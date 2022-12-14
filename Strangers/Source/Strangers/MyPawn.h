// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPawn.generated.h"

UCLASS()
class STRANGERS_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Collision)
		UCapsuleComponent* Capsule;//충돌처리

	UPROPERTY(VisibleAnywhere, Category = Visual)
		USkeletalMeshComponent* Mesh;//에셋관련

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UFloatingPawnMovement* Movement;//움직임관련

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;//카메라구도 관련

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;//카메라


private:
	void UpDown(float NewAxisValue); //폰 상하좌우 이동 관련 함수.
	void LeftRight(float NewAxisValue);

};
