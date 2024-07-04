// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"

#include "ArchVizConstructionActor.generated.h"

/**
 * 
 */
class AArchVizPlayerController;
UCLASS()
class EXPLORERTOOL_API AArchVizConstructionActor : public AArchVizActor
{
	GENERATED_BODY()
	

	bool bIsMoving = false;
	bool bIsConstructed = false;

	AArchVizPlayerController* PlayerController;
public:
	AArchVizConstructionActor();
	const int32 GridSize = 1000;
	const int32 GridCellSize = 200;
	virtual bool GetIsMoving();
	virtual bool GetIsConstructed();
	virtual void SetIsMoving(bool isMoving);
	virtual void SetIsConstructed(bool isConstructed);
	virtual FVector SnapToGrid(const FVector& Location, FVector SnapValue);
	AArchVizPlayerController* GetPlayerController();
	bool IsRotateable() override;
	
	
};
