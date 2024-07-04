// Fill out your copyright notice in the Description page of Project Settings.



#include "Managers/ArchVizRoadManager.h"


//UArchVizRoadManager::UArchVizRoadManager()
//{
//
//    RoadWidget = nullptr;
//    CurrentRoadState = ERoadState::StartRoad;
//    //CurrentSelectedActor = nullptr;
//}

void UArchVizRoadManager::SetUp()
{
	
    ChangeRoadState(ERoadState::StartRoad);
    RoadWidget = nullptr;
    CurrentSelectedActor = nullptr;
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
    if (CurrentSelectedActor)
    {
        if (RoadState == ERoadState::StartRoad || RoadState == ERoadState::EndRoad)
        {
            CurrentSelectedActor = nullptr;
        }
    }
    CurrentRoadState = RoadState;
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
void UArchVizRoadManager::MouseClicked(FHitResult HitResult)
{
    if (CurrentRoadState == ERoadState::StartRoad)
    {
        CurrentSelectedActor = GetWorld()->SpawnActor<AArchVizRoadActor>();
        CurrentSelectedActor->AddRoadPoint(HitResult.Location);
        RoadConstructionActors.Add(CurrentSelectedActor);
        ChangeRoadState(ERoadState::ExistingRoad);
    }else
    {
        if (AArchVizRoadActor* Actor = Cast<AArchVizRoadActor>(HitResult.GetActor())) {
            CurrentSelectedActor = Actor;
            ChangeRoadState(ERoadState::ExistingRoad);
        }
        else if (CurrentSelectedActor && CurrentRoadState == ERoadState::ExistingRoad) {
            CurrentSelectedActor->AddRoadPoint(HitResult.Location);
        }
    }

    
    
    
}


void UArchVizRoadManager::Start()
{
    ChangeRoadState(ERoadState::StartRoad);
}
