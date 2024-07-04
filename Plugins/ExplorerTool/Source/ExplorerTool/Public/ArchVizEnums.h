// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizEnums.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EArchVizMode : uint8
{
	RoadMode,
	ConstructionMode,
	InteriorMode,
	SaveLoadMode,
	None
};

UENUM(BlueprintType)
enum class ERoadType : uint8
{
	CurvedRoad UMETA(DisplayName = "Curved Road"),
	StraightRoad UMETA(DisplayName = "Straight Road")
};

UENUM(BlueprintType)
enum class ERoadState : uint8
{
	StartRoad UMETA(DisplayName = "Start Road"),
	EndRoad UMETA(DisplayName = "End Road"),
	ExistingRoad UMETA(DisplayName = "ExistingRoad")
};

UENUM(BlueprintType)
enum class ERoadMaterial : uint8
{
	YellowDottedLine UMETA(DisplayName = "Yellow Dotted Line"),
	TwoYellowLines UMETA(DisplayName = "Two Yellow Lines"),
	RoughPlain UMETA(DisplayName = "Rough Plain"),
	MixedLines UMETA(DisplayName = "Mixed Lines"),
	CrackedPlain UMETA(DisplayName = "Cracked Plain")
};

UENUM(BlueprintType)
enum class EConstructionTypes : uint8
{
	None,
	WallGeneration,
	SlabGeneration,
	DoorAddition
};
UENUM(BlueprintType)
enum class EInteriorItemType : uint8
{
	WallPlaceable,
	FloorPlaceable,
	CeilingPlaceable
};
UCLASS()
class EXPLORERTOOL_API UArchVizEnums: public UObject
{
	GENERATED_BODY()
};
