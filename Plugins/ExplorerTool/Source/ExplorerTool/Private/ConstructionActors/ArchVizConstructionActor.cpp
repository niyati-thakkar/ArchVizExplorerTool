// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionActors/ArchVizConstructionActor.h"
#include "Game/ArchVizPlayerController.h"

AArchVizConstructionActor::AArchVizConstructionActor()
{
    
}

bool AArchVizConstructionActor::GetIsMoving()
{
	return bIsMoving;
}

bool AArchVizConstructionActor::GetIsConstructed()
{
	return bIsConstructed;
}

void AArchVizConstructionActor::SetIsMoving(bool isMoving)
{
	bIsMoving = isMoving;
}

void AArchVizConstructionActor::SetIsConstructed(bool isConstructed)
{
    bIsConstructed = isConstructed;
}

FVector AArchVizConstructionActor::SnapToGrid(const FVector& Location, FVector SnapValue = FVector(200.0f,200.0f, 20.0f))
{

    // Adjust snapping behavior based on your grid size
    float GridUnit = GridCellSize;
    float SnappedX = FMath::RoundToFloat(Location.X / SnapValue.X) * SnapValue.X;
    float SnappedY = FMath::RoundToFloat(Location.Y / SnapValue.Y) * SnapValue.Y;
    float SnappedZ = FMath::RoundToFloat(Location.Z / SnapValue.Z) * SnapValue.Z;

    return FVector(SnappedX, SnappedY, SnappedZ);
    
}

AArchVizPlayerController* AArchVizConstructionActor::GetPlayerController()
{
    if(!PlayerController)
    {
        PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());
    }
    return PlayerController;
}


bool AArchVizConstructionActor::IsRotateable()
{
    return true;
}


