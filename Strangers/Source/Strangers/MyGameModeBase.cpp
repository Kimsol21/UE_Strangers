// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPawn::StaticClass(); //GameMode�� MyPawn�� Ŭ���� ���� ����.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode�� MyPlayerController�� Ŭ���� ���� ����.
}

