// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizRoadActor.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSaveTool.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FConstructedRoad
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	FString RoadMaterialPath;

	UPROPERTY()
	FString MeshComponentPath;

	UPROPERTY()
	ERoadType RoadType;
};

USTRUCT(BlueprintType)
struct FSaveSlotElement
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FConstructedRoad> RoadElements;
};


UCLASS()
class EXPLORERTOOL_API UArchVizSaveTool : public USaveGame
{
	GENERATED_BODY()


public:
	UPROPERTY()
	TMap<FString, FSaveSlotElement> GameSlots;

};
