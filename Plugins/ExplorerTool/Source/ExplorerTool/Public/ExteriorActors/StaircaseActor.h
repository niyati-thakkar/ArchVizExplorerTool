// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "ArchVizStructs.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"

#include "Materials/MaterialInterface.h"
#include "StaircaseActor.generated.h"



class AArchVizPlayerController;
UCLASS()
class EXPLORERTOOL_API AStaircaseActor : public AArchVizActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaircaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStairs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StairsDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* StairsMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* GlassMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UStaticMesh* GlassMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs Properties")
	UMaterialInterface* HandleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStairType StairType;
	AArchVizPlayerController* PlayerController;

protected:
	// Variable definitions (now protected)
	TArray<FSubStair*> OutMeshes;

	float StairSpacing;

	// Railing and handle dimensions
	float RailingWidth;
	float RailingThickness;
	float RailingDepth;
	float HandleWidth;
	float HandleLength;
	float RailingLengthOffset;
	float RailingWidthOffset;
	float RailingDepthOffset;
	float HandleDepth;
	float StepDepth;
	float HandleLengthOffset;  // Half width for left railing placement
	float HandleWidthOffset;  // Half width for right railing placement
	float HandleDepthOffset;
	virtual void OnConstruction(const FTransform& transform) override;
	
public:
	
	void CreateStaircaseComponent(int Index);
	void DestroyStairs();
	void GenerateStairs();
	void DestoryStaircaseComponent(FSubStair* obj);
	void SetWidth(float Width);
	void SetLength(float Length);
	void SetDepth(float Depth);
	void SetNumberOfStairs(float NOStairs);
	void RotateActor(FRotator ApplyRotation);

	float GetWidth()
	{
		return StairsWidth;
	}
	float GetLength()
	{
		return StairsLength;
	}
	float GetDepth()
	{
		return StairsDepth;
	}
	float GetNumberOfStairs()
	{
		return NumberOfStairs;
	}
	EStairType GetStairType()
	{
		return StairType;
	}
	void SetStairType(EStairType SType)
	{
		StairType = SType;
		GenerateStairs();
	}
	bool IsRotateable()
	{
		return true;
	}

	bool bIsMoving;
	bool GetIsMoving()
	{
		return bIsMoving;
	}
	
	void SetIsMoving(bool isMoving)
	{
		bIsMoving = isMoving;
	}
private:
	USceneComponent* SceneComponentStairs;
};



