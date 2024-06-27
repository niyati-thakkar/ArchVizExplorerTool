// ArchVizGridActor.cpp

#include "ConstructionActors/ArchVizGridActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

AArchVizGridActor::AArchVizGridActor()
{
    PrimaryActorTick.bCanEverTick = false;

    GridSize = 1000; // Default to 20x20 grid
    GridCellSize = 200.0f; // Default cell size is 1000 units

    GridHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GridHISM"));
    RootComponent = GridHISM;
}

void AArchVizGridActor::BeginPlay()
{
    Super::BeginPlay();

    CreateGrid(GridSize, GridCellSize);
}

void AArchVizGridActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AArchVizGridActor::CreateGrid(int32 GridSize_, float CellSize)
{
    //GridSize = GridSize_;
    //GridCellSize = CellSize;

    //UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EditorMeshes/PhAT_FloorBox.PhAT_FloorBox"));
    //UMaterialInterface* MeshMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/OpenWorldTemplate/LandscapeMaterial/T_GridChecker_A_Mat.T_GridChecker_A_Mat"));

    //if (CubeMesh && MeshMaterial)
    //{
    //    GridHISM->SetStaticMesh(CubeMesh);
    //    GridHISM->SetMaterial(0, MeshMaterial);

    //    // Calculate the offset to center the grid
    //    float GridHalfSize = (GridSize * CellSize) / 2.0f;
    //    FVector GridOffset = FVector(GridHalfSize, GridHalfSize, 0);

    //    for (int32 i = 0; i < GridSize; ++i)
    //    {
    //        for (int32 j = 0; j < GridSize; ++j)
    //        {
    //            FVector CellLocation(i * GridCellSize, j * GridCellSize, 10); // Adjust Z as needed
    //            FVector AdjustedLocation = CellLocation - GridOffset;
    //            SpawnGridCell(AdjustedLocation);
    //        }
    //    }
    //}
}

void AArchVizGridActor::SpawnGridCell(const FVector& Location)
{
    //FVector Scale = FVector(0.10f, 0.10f, 0.10f); // 10% of the original Z 
    FVector Scale = FVector(1.0f, 1.0f, 1.0f); // 10% of the original Z scale
    FTransform SpawnTransform(FRotator::ZeroRotator, Location, Scale);
    GridHISM->AddInstance(SpawnTransform);
}


void AArchVizGridActor::ShowGrid()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void AArchVizGridActor::HideGrid()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    // Destroy all spawned border cylinders
    for (AActor* BorderActor : SpawnedBorders)
    {
        if (BorderActor)
        {
            BorderActor->Destroy();
        }
    }
    SpawnedBorders.Empty();
    SpawnedCylinderLocations.Empty();
}
//void AArchVizGridActor::GenerateWall(const FVector& Location)
//{
//    FVector SnappedLocation = SnapToGrid(Location);
//
//    if (ClickedPoints.Num() > 0 && ClickedPoints.Num() % 2 == 0)
//    {
//        FVector StartPoint = ClickedPoints.Last(); // Last clicked point
//        FVector EndPoint = SnappedLocation;
//
//        // Align the new point with the X or Y coordinate of the last point
//
//        if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
//        {
//            EndPoint.Y = StartPoint.Y; // Snap to the Y axis of the last point
//        }
//        else
//        {
//            EndPoint.X = StartPoint.X; // Snap to the X axis of the last point
//        }
//
//        // Spawn walls between aligned points
//        SpawnWallBetweenPoints(StartPoint, EndPoint);
//        SnappedLocation = EndPoint;
//    }
//
//    ClickedPoints.Add(SnappedLocation); // Add snapped location to clicked points
//}

FVector AArchVizGridActor::SnapToGrid(const FVector& Location)
{
    FVector GridCenter = this->GetActorLocation();
    FVector GridOffset = FVector(GridSize * GridCellSize * 0.5f, GridSize * GridCellSize * 0.5f, 0.0f);
    FVector GridStart = GridCenter - GridOffset;

    int32 XIndex = FMath::FloorToInt((Location.X - GridStart.X) / GridCellSize);
    int32 YIndex = FMath::FloorToInt((Location.Y - GridStart.Y) / GridCellSize);
    int32 ZIndex = FMath::RoundToInt(Location.Z / 5.0f); // Snapping Z coordinate to nearest multiple of 5

    // Ensure the indices are within grid bounds
    if (XIndex >= 0 && XIndex < GridSize && YIndex >= 0 && YIndex < GridSize)
    {
        FVector GridPointLocation = GridStart + FVector((XIndex + 0.5f) * GridCellSize, (YIndex + 0.5f) * GridCellSize, ZIndex * 5.0f);
        return GridPointLocation;
    }
    return FVector::ZeroVector;
}




void AArchVizGridActor::SpawnSlabBetweenPoints(const FVector& StartPoint, const FVector& EndPoint)
{
   
        // Calculate the number of slabs needed based on the distance
        float Distance = (EndPoint - StartPoint).Size();
        int NumberOfSlabs = FMath::CeilToInt(Distance / GridCellSize);

        // Determine the maximum difference in X and Y
        float DeltaX = FMath::Abs(EndPoint.X - StartPoint.X);
        float DeltaY = FMath::Abs(EndPoint.Y - StartPoint.Y);

        // Calculate the direction vector between StartPoint and EndPoint
        FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

        // Round the direction to the nearest cardinal direction (multiples of 90 degrees)
        FVector RoundedDirection;
        if (DeltaX >= DeltaY)
        {
            // Horizontal direction (east or west)
            RoundedDirection = FVector(Direction.X, 0.0f, 0.0f).GetSafeNormal();
        }
        else
        {
            // Vertical direction (north or south)
            RoundedDirection = FVector(0.0f, Direction.Y, 0.0f).GetSafeNormal();
        }

        // Calculate the rotation based on the rounded direction
        FRotator Rotation = RoundedDirection.Rotation();

        // Spawn slabs
        for (int i = 0; i < NumberOfSlabs; ++i)
        {
            FVector SpawnLocation = StartPoint + RoundedDirection * i * GridCellSize;

            AArchVizSlabActor* NewSlab = GetWorld()->SpawnActor<AArchVizSlabActor>(AArchVizSlabActor::StaticClass(), SpawnLocation, Rotation);

            
        }
    }

 void AArchVizGridActor::GenerateSlab(const FVector& Location)
{
    FVector SnappedLocation = SnapToGrid(Location);

    // Check if there are at least two clicked points to form a slab segment
    if (ClickedPoints.Num() > 0 && ClickedPoints.Num() % 2 == 0)
    {
        FVector StartPoint = ClickedPoints[ClickedPoints.Num() - 1];
        FVector EndPoint = SnappedLocation;

        // Align the new point with the X or Y coordinate of the last point
        if (FMath::Abs(StartPoint.X - EndPoint.X) < FMath::Abs(StartPoint.Y - EndPoint.Y))
        {
            EndPoint.Y = StartPoint.Y;
        }
        else
        {
            EndPoint.X = StartPoint.X;
        }

        // Spawn slabs between points
        SpawnSlabBetweenPoints(StartPoint, EndPoint);

        // Update the snapped location to the end point for next interaction
        SnappedLocation = EndPoint;
    }

    // Add the snapped location to the list of clicked points
    ClickedPoints.Add(SnappedLocation);
}


void AArchVizGridActor::UpdateGrid(const FVector& ClickLocation)
{
    FVector SnappedLocation = SnapToGrid(ClickLocation);

    if (ClickedPoints.Num() > 1 && ClickedPoints.Num() % 2 == 0)
    {
        FVector LastPoint = ClickedPoints.Last();

        // Align the new point with the X or Y coordinate of the last point
        if (FMath::Abs(SnappedLocation.X - LastPoint.X) < FMath::Abs(SnappedLocation.Y - LastPoint.Y))
        {
            SnappedLocation.X = LastPoint.X;
        }
        else
        {
            SnappedLocation.Y = LastPoint.Y;
        }
    }

    ClickedPoints.Add(SnappedLocation);

    if (ClickedPoints.Num() > 1)
    {
        FVector StartPoint = ClickedPoints[ClickedPoints.Num() - 2];
        FVector EndPoint = ClickedPoints.Last();

        // Spawn borders between points
        SpawnBordersBetweenPoints(StartPoint, EndPoint);
    }
}
// Spawn Borders Between Points
void AArchVizGridActor::SpawnBordersBetweenPoints(const FVector& StartPoint, const FVector& EndPoint)
{
    FRotator BorderRotation = FRotator::ZeroRotator;

    // Determine the direction and rotation based on the points
    if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
    {
        // Horizontal movement
        BorderRotation.Yaw = 90.0f;
    }
    else
    {
        // Vertical movement
        BorderRotation.Yaw = 0.0f;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    UWorld* World = GetWorld();
    if (World)
    {
        FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
        float Distance = (EndPoint - StartPoint).Size();

        if (BorderRotation.Yaw == 0.0f)
        {
            // Horizontal movement
            Direction = FVector(0.0f, 1.0f, 0.0f);
            if(StartPoint.Y > EndPoint.Y)
            {
                Direction.Y = -1.0f;
            }
        }
        else
        {
            // Vertical movement
            Direction = FVector(1.0f, 0.0f, 0.0f);
            if (StartPoint.X > EndPoint.X)
            {
                Direction.X = -1.0f;
            }
        }



        // Calculate the number of border actors needed
        int32 NumBorders = FMath::RoundToInt(Distance / GridCellSize);

        for (int32 i = 0; i < NumBorders; ++i)
        {
            FVector CurrentLocation = StartPoint + Direction * (i * GridCellSize);

            AArchVizBorderActor* NewBorder = World->SpawnActor<AArchVizBorderActor>(AArchVizBorderActor::StaticClass(), CurrentLocation, BorderRotation, SpawnParams);
            if (NewBorder)
            {
                // Add the actor to the array of spawned borders
                SpawnedBorders.Add(NewBorder);
            }
        }
    }
}
//// Spawn Borders Between Points
//void AArchVizGridActor::SpawnBordersBetweenPoints(const FVector& StartPoint, const FVector& EndPoint)
//{
//    FRotator BorderRotation = FRotator::ZeroRotator;
//
//    // Determine the direction and rotation based on the points
//    if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
//    {
//        // Horizontal movement
//        BorderRotation.Yaw = 90.0f;
//    }
//
//
//    FActorSpawnParameters SpawnParams;
//    SpawnParams.Owner = this;
//    SpawnParams.Instigator = GetInstigator();
//    UWorld* World = GetWorld();
//    if (World)
//    {
//        FVector CurrentLocation = StartPoint;
//        FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
//        float Distance = (EndPoint - StartPoint).Size();
//
//        // Ensure that the direction is either horizontal or vertical
//        if (BorderRotation.Yaw == 0.0f)
//        {
//            // Horizontal movement
//            Direction = FVector(0.0f, 1.0f, 0.0f);
//        }
//        else
//        {
//            // Vertical movement
//            Direction = FVector(1.0f, 0.0f, 0.0f);
//        }
//
//        // Spawn border actors along the path
//        while ((CurrentLocation - StartPoint).Size() < Distance)
//        {
//            AArchVizBorderActor* NewBorder = World->SpawnActor<AArchVizBorderActor>(AArchVizBorderActor::StaticClass(), CurrentLocation, BorderRotation, SpawnParams);
//            if (NewBorder)
//            {
//                // Add the actor to the array of spawned borders
//                SpawnedBorders.Add(NewBorder);
//            }
//
//            // Move to the next position along the direction
//            CurrentLocation += Direction * GridCellSize;
//        }
//
//        // Ensure the last border is placed exactly at the end point
//        AArchVizBorderActor* LastBorder = World->SpawnActor<AArchVizBorderActor>(AArchVizBorderActor::StaticClass(), EndPoint, BorderRotation, SpawnParams);
//        if (LastBorder)
//        {
//            SpawnedBorders.Add(LastBorder);
//        }
//    }
//}

// 


//void AArchVizGridActor::SpawnBordersBetweenPoints(const FVector& StartPoint, const FVector& EndPoint)
//{
//    FRotator BorderRotation = FRotator::ZeroRotator;
//
//    // Correct rotation based on the direction
//    if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
//    {
//        BorderRotation.Yaw += 90.0f; // Horizontal movement
//    }
//
//    FActorSpawnParameters SpawnParams;
//    SpawnParams.Owner = this;
//    SpawnParams.Instigator = GetInstigator();
//    UWorld* World = GetWorld();
//    if (World)
//    {
//        FVector CurrentLocation = StartPoint;
//        FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
//        float Distance = (EndPoint - StartPoint).Size();
//
//        // Spawn border actors along the path
//        while ((CurrentLocation - StartPoint).Size() < Distance)
//        {
//            AArchVizBorderActor* NewBorder = World->SpawnActor<AArchVizBorderActor>(AArchVizBorderActor::StaticClass(), CurrentLocation, BorderRotation, SpawnParams);
//            if (NewBorder)
//            {
//                // Add the actor to the array of spawned borders
//                SpawnedBorders.Add(NewBorder);
//            }
//
//            // Move to the next position along the direction
//            if (BorderRotation.Yaw != 90.0f) // Horizontal
//            {
//                CurrentLocation.X += GridCellSize;
//            }
//            else // Vertical
//            {
//                CurrentLocation.Y += GridCellSize;
//            }
//        }
//    }
//}

void AArchVizGridActor::RemoveLastCylinder()
{
    if (SpawnedBorders.Num() > 0)
    {
        AActor* LastCylinderActor = SpawnedBorders.Pop();
        if (LastCylinderActor)
        {
            LastCylinderActor->Destroy();
        }
    }
}
