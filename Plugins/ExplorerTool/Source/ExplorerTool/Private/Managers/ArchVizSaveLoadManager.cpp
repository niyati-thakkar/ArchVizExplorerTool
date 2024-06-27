// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ArchVizSaveLoadManager.h"

#include "EngineUtils.h"
#include "SaveLoad/ArchVizGameInstanceSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "Tools/UEdMode.h"

void UArchVizSaveLoadManager::SetUp()
{
    SaveSlotName = "GameSlotOne";
}

void UArchVizSaveLoadManager::End()
{
	
}
void UArchVizSaveLoadManager::FloorHit(FHitResult HitResult)
{
}

void UArchVizSaveLoadManager::ActorHit(AArchVizActor* ActorSelected)
{
}

void UArchVizSaveLoadManager::Start()
{
	Super::Start();
}
void UArchVizSaveLoadManager::SaveRoad(TArray<AArchVizRoadActor*> RoadConstructionActors)
{
    if (UArchVizGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
    {
        FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
        if (CurrentSaveSlotName.IsEmpty())
        {
            Subsystem->CreateSaveGameSlot("GameSlotOne");
            CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
        }

        if (!CurrentSaveSlotName.IsEmpty())
        {
            FSaveSlotElement& SaveSlot = Subsystem->GetCurrentSaveGameSlot()->GameSlots.FindOrAdd(CurrentSaveSlotName);
            SaveSlot.RoadElements.Empty(); // Clear previous data before saving new data

            for (AArchVizRoadActor* RoadActor : RoadConstructionActors)
            {
                if (RoadActor)
                {
                    FConstructedRoad SaveSlotData;
                    USplineComponent* SplineComponent = RoadActor->SplineComponent;

                    for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
                    {
                        SaveSlotData.SplinePoints.Add(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local));
                    }

                    SaveSlotData.RoadType = RoadActor->RoadType;

                    if (UMaterialInterface* RoadMaterial = RoadActor->RoadMaterial)
                    {
                        SaveSlotData.RoadMaterialPath = RoadMaterial->GetPathName();
                    }

                    if (UStaticMesh* RoadMesh = RoadActor->RoadMesh)
                    {
                        SaveSlotData.MeshComponentPath = RoadMesh->GetPathName();
                    }

                    SaveSlot.RoadElements.Add(SaveSlotData);
                }
            }

            Subsystem->SaveGame();
        }
    }
}

void UArchVizSaveLoadManager::LoadRoad(TArray<AArchVizRoadActor*> RoadConstructionActors)
{
    if (UArchVizGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
    {
        FString SlotName = SaveSlotName;
        Subsystem->LoadGame(SlotName);
        FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();

        if (!CurrentSaveSlotName.IsEmpty())
        {
            UArchVizSaveTool* CurrentSaveGame = Subsystem->GetCurrentSaveGameSlot();
            if (FSaveSlotElement* SaveSlot = CurrentSaveGame->GameSlots.Find(SlotName))
            {
                // Destroy existing spawned actors
                TArray<AActor*> ActorsToDestroy;
                for (TActorIterator<AActor> It(GetWorld()); It; ++It)
                {
                    if (It->ActorHasTag("SpawnedActor"))
                    {
                        ActorsToDestroy.Add(*It);
                    }
                }

                for (AActor* Actor : ActorsToDestroy)
                {
                    Actor->Destroy();
                }

                RoadConstructionActors.Empty();

                // Recreate road actors from saved data
                for (const FConstructedRoad& SaveData : SaveSlot->RoadElements)
                {
                    AArchVizRoadActor* NewRoadActor = GetWorld()->SpawnActor<AArchVizRoadActor>();
                    RoadConstructionActors.Add(NewRoadActor);
 
                    USplineComponent* SplineComponent = NewRoadActor->SplineComponent;
                    SplineComponent->ClearSplinePoints();

                    for (const FVector& Point : SaveData.SplinePoints)
                    {
                        SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::Local);
                    }

                    NewRoadActor->RoadType = SaveData.RoadType;

                    if (!SaveData.RoadMaterialPath.IsEmpty())
                    {
                        UMaterialInterface* RoadMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *SaveData.RoadMaterialPath));
                        NewRoadActor->RoadMaterial = RoadMaterial;
                    }

                    if (!SaveData.MeshComponentPath.IsEmpty())
                    {
                        UStaticMesh* RoadMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *SaveData.MeshComponentPath));
                        NewRoadActor->RoadMesh = RoadMesh;
                    }

                    NewRoadActor->UpdateRoadMeshes();
                }
            }
        }
    }
}
