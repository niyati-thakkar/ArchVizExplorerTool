// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizEnums.h"
#include "ArchVizStructs.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FSubStair
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StairComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightGlassRailing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftGlassRailing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RightGlassRailingTopWood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LeftGlassRailingTopWood;

};

USTRUCT(BlueprintType)
struct FStairsSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform StairTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStairType StairType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsWidth;

};


USTRUCT(BlueprintType)
struct FStairs_DA
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStairType StairType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StairName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* StairPreview;

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
struct FConstructionElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConstructionTypes ConstructionType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ConstructionElementName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ElementPreview;
};

USTRUCT(BlueprintType)
struct FConstructionWallMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* WallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WallMaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WallMaterialPreview;
};
USTRUCT(BlueprintType)
struct FConstructionFloorMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FloorMaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* FloorMaterialPreview;
};

USTRUCT(BlueprintType)
struct FConstructionRoofMaterial
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* RoofMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText RoofMaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* RoofMaterialPreview;
};

USTRUCT(BlueprintType)
struct FInteriorItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MeshPreview;
};
USTRUCT(BlueprintType)
struct FConstructedFurniture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteriorItemType InteriorType;
};
USTRUCT(BlueprintType)
struct FConstructedWall
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfWalls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> DoorWallIndices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FConstructedFurniture> AttachedActors;
};



USTRUCT(BlueprintType)
struct FConstructedSlab
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoofMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FConstructedFurniture> AttachedActors;
};

UCLASS()
class EXPLORERTOOL_API UArchVizStructs : public UObject
{
GENERATED_BODY()
};
