// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstructionActors/ArchVizConstructionActor.h"
#include "ArchVizEnums.h"
#include "ArchVizManager.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "ArchVizConstructionManager.generated.h"

/**
 * 
 */

UCLASS()
class EXPLORERTOOL_API UArchVizConstructionManager : public UArchVizManager
{
	GENERATED_BODY()
	bool bIsFoorSelected = false;
	

public:
	void DeleteButtonClicked();
	void SetUp() override;
	void End() override;
	void MouseClicked(FHitResult HitResult) override;
	void ChangeConstructionType(EConstructionTypes ConstructionType);
	void Start() override;
	void ChangeWallMaterial(UMaterialInterface* Material);
	void ChangeFloorMaterial(UMaterialInterface* Material);
	void ChangeRoofMaterial(UMaterialInterface* Material);
	void ApplyRotation(FRotator InRotation) override;
	EConstructionTypes CurrentConstructionType;

	AArchVizConstructionActor* CurrentActor;

	TArray<AArchVizWallActor*> WallActors;
	TArray<AArchVizSlabActor*> SlabActors;
};
