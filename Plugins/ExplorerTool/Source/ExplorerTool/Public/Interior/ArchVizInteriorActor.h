// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "ArchVizEnums.h"
#include "ConstructionActors/ArchVizConstructionActor.h"

#include "ArchVizInteriorActor.generated.h"

/**
 * 
 */
class AArchVizPlayerController;
UCLASS()
class EXPLORERTOOL_API AArchVizInteriorActor : public AArchVizActor
{
	GENERATED_BODY()

	void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
public:
	AArchVizInteriorActor();
	bool bIsMoving = false;
	bool GetIsMoving()
	{
		return bIsMoving;
	}
	virtual void SetIsMoving(bool isMoving);
	UStaticMesh* GetStaticMesh()
	{

		return Mesh;
	}
	UStaticMeshComponent* MeshComponent;
	UStaticMesh* Mesh;
	EInteriorItemType ComponentType;
	AArchVizPlayerController* PlayerController;
	void SetStaticMesh(UStaticMesh* StaticMesh, EInteriorItemType ItemType);
	void AdjustPositionForPlacement();
	void RotateActor(FRotator ApplyRotation = FRotator(0, 90, 0)) override;
	EInteriorItemType GetComponentType();
	bool IsRotateable() override;

};
