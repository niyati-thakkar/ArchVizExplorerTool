// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizStructs.h"
#include "Engine/DataAsset.h"
#include "TaskBarDataAsset.generated.h"

/**
 * 
 */


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Logo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EArchVizMode, FMasterWidgetButtonProp> MasterWidgetContentMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConstructionWallMaterial> ConstructionWallMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< FConstructionFloorMaterial> ConstructionFloorMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray< FConstructionRoofMaterial> ConstructionRoofMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConstructionElement> ConstructionElements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteriorItem> CeilingPlaceables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteriorItem> FloorPlaceables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteriorItem> WallPlaceables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SavedSlotsLogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SaveSlotIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* LoadSlotIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* NewSlotIcon;
};
 