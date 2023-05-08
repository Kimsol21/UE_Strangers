// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/MyBossGameModeBase.h"
#include "Character/Player/MyPlayer.h"
#include "Character/Player/MyPlayerController.h"


AMyBossGameModeBase::AMyBossGameModeBase()
{
	DefaultPawnClass = AMyPlayer::StaticClass(); //GameMode에 MyCharacter의 클래스 정보 지정.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode에 MyPlayerController의 클래스 정보 지정.
}
