// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ArchVizActor.h"
#include "ArchVizSlabActor.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API AArchVizSlabActor : public AArchVizActor
{
	GENERATED_BODY()

	
public:
	AArchVizSlabActor();

	UStaticMeshComponent* SlabComponent;

	UStaticMesh* SlabMesh;
};
