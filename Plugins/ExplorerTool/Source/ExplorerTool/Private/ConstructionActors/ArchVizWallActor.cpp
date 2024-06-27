// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionActors/ArchVizWallActor.h"

// Sets default values
AArchVizWallActor::AArchVizWallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = SceneComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshAsset(TEXT("/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400"));
	if (WallMeshAsset.Succeeded())
	{
		WallMesh = WallMeshAsset.Object;
	}
	
}

// Called when the game starts or when spawned
void AArchVizWallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArchVizWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AArchVizWallActor::SpawnWallBetweenPoints(const FVector& StartPoint, const FVector& EndPoint)
//{
//    // Calculate the number of walls needed based on the distance
//    float Distance = (EndPoint - StartPoint).Size();
//    int NumberOfWalls = FMath::CeilToInt(Distance / GridCellSize);
//
//    // Calculate the direction vector between StartPoint and EndPoint
//    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
//
//    // Round the direction to the nearest cardinal direction (multiples of 90 degrees)
//    FVector RoundedDirection;
//    if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Y))
//    {
//        // Horizontal direction (east or west)
//        RoundedDirection = FVector(FMath::RoundToInt(Direction.X), 0.0f, 0.0f).GetSafeNormal();
//    }
//    else
//    {
//        // Vertical direction (north or south)
//        RoundedDirection = FVector(0.0f, FMath::RoundToInt(Direction.Y), 0.0f).GetSafeNormal();
//    }
//
//    // Calculate the rotation based on the rounded direction
//    FRotator Rotation = RoundedDirection.Rotation();
//
//    // Spawn walls
//    for (int i = 0; i < NumberOfWalls; ++i)
//    {
//        FVector SpawnLocation = StartPoint + RoundedDirection * i * GridCellSize;
//
//        // Offset the spawn location slightly to overlap with the previous wall
//        if (i > 0)
//        {
//            SpawnLocation -= RoundedDirection * GridCellSize * 0.01f; // Adjust overlap distance as needed
//        }
//
//        AArchVizWallActor* NewWall = GetWorld()->SpawnActor<AArchVizWallActor>(AArchVizWallActor::StaticClass(), SpawnLocation, Rotation);
//        if (NewWall)
//        {
//            FVector Offset = FVector::ZeroVector;
//
//            // Adjust the offset based on the rotation angle
//            float WallWidth = NewWall->GetWallWidth();
//            float Yaw = Rotation.Yaw;
//
//            if (FMath::Abs(Yaw) <= 5 || FMath::Abs(Yaw - 360) <= 5)
//            {
//                Offset.X = WallWidth / 2.0f;
//            }
//            else if (FMath::Abs(Yaw - 180) <= 5)
//            {
//                Offset.X = -WallWidth / 2.0f;
//            }
//            else if (FMath::Abs(Yaw - 90) <= 5)
//            {
//                Offset.Y = WallWidth / 2.0f;
//            }
//            else if (FMath::Abs(Yaw + 90) <= 5)
//            {
//                Offset.Y = -WallWidth / 2.0f;
//            }
//
//            // Apply the offset
//            NewWall->SetActorLocation(SpawnLocation + Offset);
//        }
//    }
//}


