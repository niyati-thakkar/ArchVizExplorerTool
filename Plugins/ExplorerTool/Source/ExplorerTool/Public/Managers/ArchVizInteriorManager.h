// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizManager.h"
#include "ArchVizInteriorManager.generated.h"

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
	void FloorHit(FHitResult HitResult) override;
	void ActorHit(AArchVizActor* ActorSelected) override;
	void Start() override;
};