// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TaskBarDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EButtonCategory : uint8
{
	Roads UMETA(DisplayName = "Roads"),
	Infrastructure UMETA(DisplayName = "Infrastructure"),
	Interior UMETA(DisplayName = "Interior"),
	Materials UMETA(DisplayName = "Materials"),
	SaveLoad UMETA(DisplayName = "Save/Load")
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


USTRUCT(BlueprintType)
struct FRoadType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText RoadTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoadType RoadType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* RoadTypePreview;

};

USTRUCT(BlueprintType)
struct FRoadState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText RoadStateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoadState RoadState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* RoadStatePreview;

};

USTRUCT(BlueprintType)
struct FRoadMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText RoadMaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* RoadMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* RoadMaterialPreview;

};


UCLASS(BlueprintType)
class EXPLORERTOOL_API UTaskBarDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoadType> RoadTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoadState> RoadStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoadMaterial> RoadMaterials;

};
