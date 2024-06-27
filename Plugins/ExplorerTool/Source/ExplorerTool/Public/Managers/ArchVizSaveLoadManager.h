// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.h"
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
	void FloorHit(FHitResult HitResult) override;
	void ActorHit(AArchVizActor* ActorSelected) override;
	void Start() override;
	void SaveRoad(TArray<AArchVizRoadActor*> RoadConstructionActors);
	void LoadRoad(TArray<AArchVizRoadActor*> RoadConstructionActors);
	UPROPERTY()
	FString SaveSlotName;
};