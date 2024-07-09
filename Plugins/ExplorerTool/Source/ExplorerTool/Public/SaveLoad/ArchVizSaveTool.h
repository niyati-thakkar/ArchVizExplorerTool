// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizStructs.h"
#include "ArchVizSaveTool.generated.h"

/**
 * 
 */




UCLASS()
class EXPLORERTOOL_API UArchVizSaveTool : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY()

	TArray<FConstructedRoad> ConstructedRoads;

	UPROPERTY()
	TArray<FConstructedSlab> ConstructedSlabs;

	UPROPERTY()
	TArray<FConstructedWall> ConstructedWalls;

	UPROPERTY()
	TArray<FStairsSaveData> ConstructedStairs;

};
