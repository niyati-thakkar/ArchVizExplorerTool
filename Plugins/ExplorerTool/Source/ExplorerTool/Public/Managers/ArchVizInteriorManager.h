// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.h"
#include "Interior/ArchVizInteriorActor.h"
#include "ArchVizInteriorManager.generated.h"

class UInteriorWidget;
/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UArchVizInteriorManager:public UArchVizManager
{
	GENERATED_BODY()
public:
	void SetUp() override;
	void End() override;
	void MouseClicked(FHitResult HitResult) override;
	void Start() override;
	void AttachToCeiling(UStaticMesh* StaticMesh);
	void PlaceOnFloor(UStaticMesh* StaticMesh);
	void PlaceOnWall(UStaticMesh* StaticMesh);
	void DeleteButtonClicked();
	void PlaceActor(FHitResult HitResult);
	void ApplyRotation(FRotator InRotation) override;
	void UpdateUI();
	AArchVizInteriorActor* CurrentActor;
	TArray<AArchVizInteriorActor*> InteriorActors;
	UInteriorWidget* InteriorWidget;
};