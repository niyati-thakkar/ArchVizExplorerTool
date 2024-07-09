// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "ExteriorActors/StaircaseActor.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "ArchVizSaveLoadManager.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UArchVizSaveLoadManager :public UArchVizManager
{
	GENERATED_BODY()
public:
	void SetUp() override;
	void End() override;
	void MouseClicked(FHitResult HitResult) override;
	void Start() override;
	void CreateSlotIfNotExists(FString NewSlotName);
	void SaveSlot(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<AArchVizWallActor*>& WallConstructionActors, TArray<
	              AArchVizSlabActor*>& SlabConstructionActors, TArray<AStaircaseActor*> SpawnedStairs);
	void LoadSlot(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<AArchVizWallActor*>& WallConstructionActors, TArray<
	              AArchVizSlabActor*>& SlabConstructionActors, TArray<AStaircaseActor*> StairConstructionActors);
	void SaveStairs(TArray<AStaircaseActor*>& StairConstructionActors, TArray<FStairsSaveData>& ConstructedStairs);
	void LoadStairs(TArray<AStaircaseActor*>& SlabConstructionActors, const TArray<FStairsSaveData>& ConstructedSlabs);

	void SaveRoad(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<FConstructedRoad>& ConstructedRoads);
	void LoadRoad(TArray<AArchVizRoadActor*>& RoadConstructionActors, const TArray<FConstructedRoad>& ConstructedRoads);

	void SaveWall(TArray<AArchVizWallActor*>& WallConstructionActors, TArray<FConstructedWall>& ConstructedWalls);
	void LoadWall(TArray<AArchVizWallActor*>& WallConstructionActors, const TArray<FConstructedWall>& ConstructedWalls);

	void SaveSlab(TArray<AArchVizSlabActor*>& SlabConstructionActors, TArray<FConstructedSlab>& ConstructedSlabs);
	void LoadSlab(TArray<AArchVizSlabActor*>& SlabConstructionActors, const TArray<FConstructedSlab>& ConstructedSlabs);

	FString GetSlotName()
	{
		return SaveSlotName;
	}
	void SetSlotName(FString NewSlotName)
	{
		SaveSlotName = NewSlotName;
	}
	TArray<FConstructedFurniture> GetInteriorItems(AArchVizConstructionActor* Actor);
	UPROPERTY()
	FString SaveSlotName;
};