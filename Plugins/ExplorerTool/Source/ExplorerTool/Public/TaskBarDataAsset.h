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



};
 