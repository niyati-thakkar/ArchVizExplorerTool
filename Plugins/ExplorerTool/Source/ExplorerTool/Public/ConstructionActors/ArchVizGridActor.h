// ArchVizGridActor.h
#pragma once

#include "CoreMinimal.h"
#include "../ArchVizActor.h"
#include "ArchVizBorderActor.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "ArchVizGridActor.generated.h"

UCLASS()
class EXPLORERTOOL_API AArchVizGridActor : public AArchVizActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AArchVizGridActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Function to create the grid
    UFUNCTION(BlueprintCallable)
    void CreateGrid(int32 GridSize_, float CellSize);

    void GenerateSlab(const FVector& Location);
    // Function to update the grid position based on click location
    UFUNCTION(BlueprintCallable)
    void UpdateGrid(const FVector& ClickLocation);
    void SpawnBordersBetweenPoints(const FVector& StartPoint, const FVector& EndPoint);

    // Functions to show and hide the grid
    UFUNCTION(BlueprintCallable)
    void ShowGrid();

    UFUNCTION(BlueprintCallable)
    void HideGrid();
    FVector SnapToGrid(const FVector& Location);
    void SpawnWallBetweenPoints(const FVector& Vector, const FVector& EndPoint);
    //void GenerateWall(const FVector& Location);
    void SpawnSlabBetweenPoints(const FVector& StartPoint, const FVector& EndPoint);

    bool IsWithinGridBounds(const FVector& Location);
    // Function to spawn a cylinder at a given location


    // Function to remove the last spawned cylinder
    UFUNCTION(BlueprintCallable)
    void RemoveLastCylinder();
    int32 GridSize;
    float GridCellSize;
    bool bIsGridVisible = false;
    UPROPERTY()
    TArray<AArchVizBorderActor*> SpawnedBorders;
    UPROPERTY()
    TArray<FVector> ClickedPoints;

private:


    // Hierarchical Instanced Static Mesh component for the grid cells
    UPROPERTY()
    class UHierarchicalInstancedStaticMeshComponent* GridHISM;

    // Array to hold the locations of the spawned cylinders
    TArray<FVector> SpawnedCylinderLocations;

    // Static mesh for the cylinder


    // Function to spawn a grid cell
    void SpawnGridCell(const FVector& Location);
};

