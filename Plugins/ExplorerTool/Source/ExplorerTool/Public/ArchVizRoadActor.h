// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "TaskBarDataAsset.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ArchVizRoadActor.generated.h"



UCLASS()
class EXPLORERTOOL_API AArchVizRoadActor : public AArchVizActor
{
	GENERATED_BODY()

	
public:
	AArchVizRoadActor();

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
	void RemoveLastPoint();


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoadState RoadState;
};