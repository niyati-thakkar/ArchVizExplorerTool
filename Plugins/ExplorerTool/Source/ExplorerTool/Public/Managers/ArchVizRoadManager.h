// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizEnums.h"
#include "ArchVizManager.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "ArchVizRoadManager.generated.h"
class URoadWidget;
/**
 * 
 */

UCLASS()
class EXPLORERTOOL_API UArchVizRoadManager :public UArchVizManager
{
	GENERATED_BODY()
public:
	//UArchVizRoadManager();
	void SetUp() override;
	void End() override;
	void MouseClicked(FHitResult HitResult) override;
	void Start() override;
	UFUNCTION(BlueprintCallable)
	void ChangeRoadType(ERoadType RoadType);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadState(ERoadState RoadState);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadWidth(int RoadWidth);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadMaterial(UMaterialInterface* NewMaterial);
	UFUNCTION(BlueprintCallable)
	void DeleteRoad();
	UFUNCTION(BlueprintCallable)
	void RemoveLastSplinePoint();


	URoadWidget* RoadWidget;

	AArchVizRoadActor* CurrentSelectedActor;

	UPROPERTY()
	TArray<AArchVizRoadActor*> RoadConstructionActors;

	UPROPERTY()
	TArray<FConstructedRoad>  RoadInfo;

	ERoadState CurrentRoadState;
};