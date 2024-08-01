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

class UConstructionWidget;

UCLASS()
class EXPLORERTOOL_API UArchVizConstructionManager : public UArchVizManager
{
	GENERATED_BODY()
	bool bIsFoorSelected = false;
	

public:
	UConstructionWidget* ConstructionWidget;
	void DeleteButtonClicked();
	void SetUp() override;
	void End() override;
	void MouseClicked(FHitResult HitResult) override;
	void HandleDoorAddition(FHitResult HitResult);
	void HandleDoorPlacement(FHitResult HitResult);
	void HandleDoorSelectionOrMove(FHitResult HitResult);
	void HandleOtherConstructionTypes(FHitResult HitResult);
	void HandleExistingActor(FHitResult HitResult);
	void HandleActorSelection(FHitResult HitResult);
	void HandleNewActor(FHitResult HitResult);
	void HandleNewWallActor(FHitResult HitResult);
	void HandleNewSlabActor(FHitResult HitResult);
	void SelectCurrentActor(FHitResult HitResult);
	void SelectConstructionTypeActor(FHitResult HitResult);
	void ChangeConstructionType(EConstructionTypes ConstructionType);
	void Start() override;
	void ChangeWallMaterial(UMaterialInterface* Material);
	void ChangeFloorMaterial(UMaterialInterface* Material);
	void ChangeRoofMaterial(UMaterialInterface* Material);
	void ApplyRotation(FRotator InRotation) override;
	void UpdateUI();
	EConstructionTypes CurrentConstructionType;

	AArchVizConstructionActor* CurrentActor;

	TArray<AArchVizWallActor*> WallActors;
	TArray<AArchVizSlabActor*> SlabActors;
};
