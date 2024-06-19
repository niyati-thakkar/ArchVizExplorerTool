// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "RoadConstructionActor.generated.h"


UENUM()
enum class ERoadType
{
	CurvedRoad,
	StraightRoad
};
UCLASS()
class EXPLORERTOOL_API ARoadConstructionActor : public AActor
{
	GENERATED_BODY()

	
public:
	ARoadConstructionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddRoadPoint(FVector Location);

	UFUNCTION()
	void UpdateRoadMeshes();
	UFUNCTION()
	void SaveRoadState(const FString& SlotName);
	UFUNCTION()
	void LoadRoadState(const FString& SlotName);

	UPROPERTY()
	USplineComponent* SplineComponent;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* RoadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* RoadMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoadWidth = 100;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoadType RoadType;
};