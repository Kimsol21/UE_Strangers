// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPlayer::StaticClass(); //GameMode�� MyCharacter�� Ŭ���� ���� ����.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode�� MyPlayerController�� Ŭ���� ���� ����.
}

