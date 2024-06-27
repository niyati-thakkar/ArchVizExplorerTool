// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "UObject/NoExportTypes.h"
#include "ArchVizManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EXPLORERTOOL_API UArchVizManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void SetUp();
	virtual void End();
	virtual void FloorHit(FHitResult HitResult);
	virtual void ActorHit(AArchVizActor* ActorSelected);
	virtual void Start();
};
