// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpinBox.h"
#include "ExteriorActors/StaircaseActor.h"
#include "Managers/ArchVizManager.h"
#include "ArchVizExteriorManager.generated.h"

class UExteriorWidget;
/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UArchVizExteriorManager : public UArchVizManager
{
	GENERATED_BODY()
public:
	void ChangeStairsWidth(float GetValue);
	void ChangeStairsLength(float GetValue);
	void ChangeStairsDepth(float GetValue);
	void ChangeNumberOfStairs(float GetValue);
	void ChangeStairsType(EStairType StairType);
	void MouseClicked(FHitResult HitResult);
	void End();
	void SetUp();
	void DeleteStairs();
	void ApplyRotation(FRotator InRotation = FRotator::ZeroRotator) override;
	void UpdateWidgetValues();
	AStaircaseActor* CurrentSelectedActor;
	UExteriorWidget* ExteriorWidget;
	TArray<AStaircaseActor*> SpawnedStairs;
};
