// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoadConstructionDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSelectableMaterial
{
	GENERATED_BODY()



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

};
UCLASS()
class EXPLORERTOOL_API URoadConstructionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSelectableMaterial> RoadMaterials;
};
