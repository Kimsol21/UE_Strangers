// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//���ӿ��� �Է°� ����.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
