// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ArchVizGameModeBase.h"

#include "Game/ArchVizPlayerController.h"

AArchVizGameModeBase::AArchVizGameModeBase()
{

	PlayerControllerClass = AArchVizPlayerController::StaticClass();
}
