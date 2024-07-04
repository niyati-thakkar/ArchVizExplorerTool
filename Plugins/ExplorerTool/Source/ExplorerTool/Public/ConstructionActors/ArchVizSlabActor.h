// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizConstructionActor.h"
#include "ProceduralMeshComponent.h"
#include "ArchVizSlabActor.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API AArchVizSlabActor : public AArchVizConstructionActor
{
	GENERATED_BODY()

	UProceduralMeshComponent* ProceduralSlabMesh;
	double SlabSizeX;
	double SlabSizeY;
	float SlabSizeZ;
	UMaterialInterface* FloorMaterial;
	UMaterialInterface* RoofMaterial;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	AArchVizSlabActor();
	void BeginPlay();
	void Tick(float DeltaTime);
	void AddStartPoint(FVector SP);
	void AddEndPoint(FVector EP);
	void SpawnSlabs();
	void GenerateCube(FVector StartLocation, int Segment);
	void GenerateSlab();
	void AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3);
	void PlaceSlab(FVector NewLocation);
	//UProceduralMeshComponent* GenerateCube(float Length, float Breadth, float Height, float SlabThickness, const FVector& SpawnLocation);
	void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent);
	UStaticMesh* SlabMesh;
	USceneComponent* SceneComponent;
	FVector StartPoint;
	UMaterialInterface* GetRoofMaterial()
	{
		return RoofMaterial;
	}
	UMaterialInterface* GetFloorMaterial()
	{
		return FloorMaterial;
	}
	FVector GetStartPoint()
	{
		return StartPoint;
	}
	FVector GetEndPoint()
	{
		return EndPoint;
	}
	FVector EndPoint;
	void SetMaterial(UMaterialInterface* Material, int Segment);
	void RotateActor(FRotator InRotation = FRotator(0, 90, 0)) override;

};
