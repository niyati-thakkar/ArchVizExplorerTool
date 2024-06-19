// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizGameModeBase.h"

#include "ArchVizPlayerController.h"

AArchVizGameModeBase::AArchVizGameModeBase()
{

	PlayerControllerClass = AArchVizPlayerController::StaticClass();
}
