// Fill out your copyright notice in the Description page of Project Settings.



#include "Managers/ArchVizRoadManager.h"


void UArchVizRoadManager::SetUp()
{
	
    ChangeRoadState(ERoadState::StartRoad);

}

void UArchVizRoadManager::End()
{
    CurrentSelectedActor = nullptr;
}
void UArchVizRoadManager::ChangeRoadType(ERoadType RoadType)
{
    if (CurrentSelectedActor)
    {
        if (AArchVizRoadActor* RoadActor = Cast<AArchVizRoadActor>(CurrentSelectedActor))
        {
            RoadActor->RoadType = RoadType;
            RoadActor->UpdateRoadMeshes();
        }
    }
}

void UArchVizRoadManager::ChangeRoadState(ERoadState RoadState)
{
    CurrentRoadState = RoadState;
    if (CurrentSelectedActor)
    {
        if (RoadState == ERoadState::EndRoad)
        {
            CurrentSelectedActor = nullptr;
        }
    }
    
}

void UArchVizRoadManager::ChangeRoadWidth(int RoadWidth)
{
    if (CurrentSelectedActor)
    {
        if (AArchVizRoadActor* RoadActor = Cast<AArchVizRoadActor>(CurrentSelectedActor))
        {
            RoadActor->RoadWidth = RoadWidth;
            RoadActor->UpdateRoadMeshes();
        }
    }
}

void UArchVizRoadManager::ChangeRoadMaterial(UMaterialInterface* NewMaterial)
{
    if (CurrentSelectedActor)
    {
        if (AArchVizRoadActor* RoadActor = Cast<AArchVizRoadActor>(CurrentSelectedActor))
        {
            RoadActor->RoadMaterial = NewMaterial;
            RoadActor->UpdateRoadMeshes();
        }
    }
}

void UArchVizRoadManager::DeleteRoad()
{
    if (CurrentSelectedActor)
    {
        if (AArchVizRoadActor* RoadActor = Cast<AArchVizRoadActor>(CurrentSelectedActor))
        {
            RoadConstructionActors.Remove(RoadActor);
            RoadActor->Destroy();
            CurrentSelectedActor = nullptr;
        }
    }
}

void UArchVizRoadManager::RemoveLastSplinePoint()
{
    if (CurrentSelectedActor)
    {
        if (AArchVizRoadActor* RoadActor = Cast<AArchVizRoadActor>(CurrentSelectedActor))
        {
            RoadActor->RemoveLastPoint();
            RoadActor->UpdateRoadMeshes();
        }
    }
}

void UArchVizRoadManager::FloorHit(FHitResult HitResult)
{
    if (CurrentSelectedActor)
    {
        CurrentSelectedActor->AddRoadPoint(HitResult.Location);
    }
    if (CurrentRoadState == ERoadState::StartRoad)
    {
        CurrentSelectedActor = GetWorld()->SpawnActor<AArchVizRoadActor>();
        RoadConstructionActors.Add(CurrentSelectedActor);
        CurrentRoadState = ERoadState::ExistingRoad;
    }
}

void UArchVizRoadManager::ActorHit(AArchVizActor* ActorSelected)
{
    CurrentSelectedActor = Cast<AArchVizRoadActor>(ActorSelected);
    ChangeRoadState(ERoadState::ExistingRoad);
}

void UArchVizRoadManager::Start()
{
    ChangeRoadState(ERoadState::StartRoad);
}
