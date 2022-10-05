// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass(); //GameMode�� MyCharacter�� Ŭ���� ���� ����.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode�� MyPlayerController�� Ŭ���� ���� ����.
}

