// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//게임에만 입력값 전달.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
