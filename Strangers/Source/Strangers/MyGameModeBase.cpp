// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPawn::StaticClass(); //GameMode에 MyPawn의 클래스 정보 지정.
	PlayerControllerClass = AMyPlayerController::StaticClass(); //GameMode에 MyPlayerController의 클래스 정보 지정.
}

