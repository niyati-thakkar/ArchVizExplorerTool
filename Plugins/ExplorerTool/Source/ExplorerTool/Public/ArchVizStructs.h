// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizEnums.h"
#include "ArchVizStructs.generated.h"

/**
 * 
 */


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

USTRUCT(BlueprintType)
struct FMasterWidgetButtonProp
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ButtonPreview;

};
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
class EXPLORERTOOL_API UArchVizStructs : public UObject
{
GENERATED_BODY()
};
