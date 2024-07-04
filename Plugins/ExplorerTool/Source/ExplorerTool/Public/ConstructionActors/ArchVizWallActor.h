// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.h"
#include "ArchVizConstructionActor.h"
#include "ArchVizWallActor.generated.h"
class AArchVizDoorWallActor;
UCLASS()
class EXPLORERTOOL_API AArchVizWallActor : public AArchVizConstructionActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArchVizWallActor();

	USceneComponent* SceneComponent;

	UStaticMesh* WallMesh;

	FVector StartPoint;

	FVector EndPoint;
	float WallWidth = 20.0f;
	float WallLenght = 400.0f;
	float WallHeight = 400.0f;
	TArray<UStaticMeshComponent*> SpawnedWalls;
	TArray<AArchVizDoorWallActor*> DoorWalls;
	int32 NumberOfWalls;
	FRotator Rotation;
	UMaterialInterface* Material;
	TArray<bool> bHasDoorWall;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddStartPoint(FVector SP);
	void AddEndPoint(FVector EP);
	void SpawnWall();
	void SpawnDoorActor(FVector SpawnLocation);
	void SpawnMesh(FVector SpawnLocation);
	void ReplaceWallWithDoor(FVector HitLocation);
	void ReplaceWallWithDoor(int32 index);
	void PlaceWall(FVector NewLocation);
	void ReplaceDoorWithWall(AArchVizDoorWallActor* DoorWallActor);
	void SetMaterial(UMaterialInterface* NewMaterial);
	FRotator GetRotation()
	{
		return Rotation;
	}
	UMaterialInterface* GetMaterial()
	{
		return Material;
	}
	FVector GetStartPoint()
	{
		return StartPoint;
	}
	FVector GetEndPoint()
	{
		return EndPoint;
	}
	int32 GetNumberOfWalls()
	{
		return NumberOfWalls;
	}
	TArray<bool>& GetDoorWallIndices()
	{
		return bHasDoorWall;
	}
	void RotateActor(FRotator InRotation = FRotator(0, 90, 0)) override;
	void HighlightSelectedActor();
	void UnhighlightDeselectedActor();
	int32 GetWallIndexFromLocation(FVector Location) const;
	FVector GetLocationFromIndex(int32 Index) const;
	//float GetWallWidth();
	//void SpawnWallBetweenPoints(const FVector& StartPoint, const FVector& EndPoint);
	//void GenerateWall(const FVector& Location);
	
};
