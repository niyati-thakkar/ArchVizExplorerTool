// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ArchVizActor.h"
#include "ArchVizWallActor.generated.h"

UCLASS()
class EXPLORERTOOL_API AArchVizWallActor : public AArchVizActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArchVizWallActor();

	USceneComponent* SceneComponent;

	UStaticMesh* WallMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//float GetWallWidth();
	//void SpawnWallBetweenPoints(const FVector& StartPoint, const FVector& EndPoint);
	//void GenerateWall(const FVector& Location);
	int32 GridCellSize;
};
