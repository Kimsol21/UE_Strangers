// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyBossGameModeBase.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"


AMyBossGameModeBase::AMyBossGameModeBase()
{
	DefaultPawnClass = AMyPlayer::StaticClass(); //GameMode�� MyCharacter�� Ŭ���� ���� ����.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode�� MyPlayerController�� Ŭ���� ���� ����.
}
