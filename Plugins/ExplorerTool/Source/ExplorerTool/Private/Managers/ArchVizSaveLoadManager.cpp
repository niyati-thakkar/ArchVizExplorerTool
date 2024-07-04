// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/ArchVizSaveLoadManager.h"


#include "EngineUtils.h"
#include "SaveLoad/ArchVizGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "Components/SplineComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Interior/ArchVizInteriorActor.h"

void UArchVizSaveLoadManager::SetUp()
{
    SaveSlotName = "GameSlotOne";
}

void UArchVizSaveLoadManager::End()
{
}

void UArchVizSaveLoadManager::MouseClicked(FHitResult HitResult)
{
}

void UArchVizSaveLoadManager::Start()
{
    Super::Start();
}

void UArchVizSaveLoadManager::CreateSlotIfNotExists(FString NewSlotName)
{
    if (UArchVizGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
    {
        FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
        if (CurrentSaveSlotName.IsEmpty() || CurrentSaveSlotName != NewSlotName)
        {
            Subsystem->CreateSaveGameSlot(NewSlotName);
            Subsystem->SaveGame();
        }
    }
}
void UArchVizSaveLoadManager::SaveSlot(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<AArchVizWallActor*>& WallConstructionActors, TArray<AArchVizSlabActor*>& SlabConstructionActors)
{
    if (UArchVizGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
    {
        FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
        if (CurrentSaveSlotName.IsEmpty() || CurrentSaveSlotName != SaveSlotName)
        {
            Subsystem->CreateSaveGameSlot(SaveSlotName);
            CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
        }
        

        if (!CurrentSaveSlotName.IsEmpty())
        {
            UArchVizSaveTool* SaveSlot = Subsystem->GetCurrentSaveGameSlot();
            SaveRoad(RoadConstructionActors, SaveSlot->ConstructedRoads);
            SaveWall(WallConstructionActors, SaveSlot->ConstructedWalls);
            SaveSlab(SlabConstructionActors, SaveSlot->ConstructedSlabs);

            Subsystem->SaveGame();
        }
    }
}

void UArchVizSaveLoadManager::LoadSlot(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<AArchVizWallActor*>& WallConstructionActors, TArray<AArchVizSlabActor*>& SlabConstructionActors)
{
    if (UArchVizGameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
    {
        FString SlotName = SaveSlotName;
        Subsystem->LoadGame(SlotName);
        FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();

        if (!CurrentSaveSlotName.IsEmpty())
        {
            UArchVizSaveTool* CurrentSaveGame = Subsystem->GetCurrentSaveGameSlot();
            LoadRoad(RoadConstructionActors, CurrentSaveGame->ConstructedRoads);
            LoadWall(WallConstructionActors, CurrentSaveGame->ConstructedWalls);
            LoadSlab(SlabConstructionActors, CurrentSaveGame->ConstructedSlabs);
        }
    }
}

void UArchVizSaveLoadManager::SaveRoad(TArray<AArchVizRoadActor*>& RoadConstructionActors, TArray<FConstructedRoad>& ConstructedRoads)
{
    ConstructedRoads.Empty();

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

            ConstructedRoads.Add(SaveSlotData);
        }
    }
}

void UArchVizSaveLoadManager::LoadRoad(TArray<AArchVizRoadActor*>& RoadConstructionActors, const TArray<FConstructedRoad>& ConstructedRoads)
{
    // Clear existing road actors
    for (AArchVizRoadActor* RoadActor : RoadConstructionActors)
    {
        RoadActor->Destroy();
    }
    RoadConstructionActors.Empty();

    // Recreate road actors from saved data
    for (const FConstructedRoad& SaveData : ConstructedRoads)
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

void UArchVizSaveLoadManager::SaveWall(TArray<AArchVizWallActor*>& WallConstructionActors, TArray<FConstructedWall>& ConstructedWalls)
{
    

    for (AArchVizWallActor* WallActor : WallConstructionActors)
    {
        if (WallActor)
        {
            FConstructedWall SaveSlotData;

            SaveSlotData.StartPoint = WallActor->StartPoint;

            SaveSlotData.EndPoint = WallActor->EndPoint;

            SaveSlotData.DoorWallIndices = WallActor->GetDoorWallIndices();

            SaveSlotData.NumberOfWalls = WallActor->GetNumberOfWalls();

            if(WallActor->GetMaterial())
            {
                SaveSlotData.WallMaterial = WallActor->GetMaterial()->GetPathName();
            }
            SaveSlotData.AttachedActors = GetInteriorItems(WallActor);

            ConstructedWalls.Add(SaveSlotData);
        }
    }
}
TArray<FConstructedFurniture> UArchVizSaveLoadManager::GetInteriorItems(AArchVizConstructionActor* Actor)
{
    TArray<AActor*> SubActors;
    TArray<FConstructedFurniture> ResultantArray;
    Actor->GetAttachedActors(SubActors);
	for(AActor* SubActor: SubActors)
	{
		if(AArchVizInteriorActor* InteriorActor = Cast<AArchVizInteriorActor>(SubActor))
		{
            FConstructedFurniture furniture;
            furniture.ActorTransform = InteriorActor->GetActorTransform();
            furniture.InteriorType = InteriorActor->GetComponentType();
            furniture.StaticMesh = InteriorActor->GetStaticMesh()->GetPathName();
            ResultantArray.Add(furniture);
		}
	}
    return ResultantArray;
}

void UArchVizSaveLoadManager::LoadWall(TArray<AArchVizWallActor*>& WallConstructionActors, const TArray<FConstructedWall>& ConstructedWalls)
{
    for (AArchVizWallActor* WallActor : WallConstructionActors)
    {
        WallActor->Destroy();
    }
    WallConstructionActors.Empty();
    for(const FConstructedWall& Wall: ConstructedWalls)
    {
        AArchVizWallActor* NewWall = GetWorld()->SpawnActor<AArchVizWallActor>();
        NewWall->AddStartPoint(Wall.StartPoint);
        UMaterialInterface* WallMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Wall.WallMaterial));
        NewWall->SetMaterial(WallMaterial);
        NewWall->AddEndPoint(Wall.EndPoint);
        NewWall->SetIsMoving(false);
        for(int i=0;i<Wall.NumberOfWalls && i < Wall.DoorWallIndices.Num();i++)
        {
	        if(Wall.DoorWallIndices[i])
	        {
                NewWall->ReplaceWallWithDoor(i);
	        }
        }
        for(int i=0;i<Wall.AttachedActors.Num();i++)
        {
            FTransform ActorTransform = Wall.AttachedActors[i].ActorTransform;
            FActorSpawnParameters SpawnParams;

            AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>(AArchVizInteriorActor::StaticClass(), ActorTransform, SpawnParams);
            UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Wall.AttachedActors[i].StaticMesh));
            InteriorActor->SetStaticMesh(StaticMesh, Wall.AttachedActors[i].InteriorType);
            InteriorActor->AttachToActor(NewWall, FAttachmentTransformRules::KeepWorldTransform);
        }
        WallConstructionActors.Add(NewWall);
        
    }
}

void UArchVizSaveLoadManager::SaveSlab(TArray<AArchVizSlabActor*>& SlabConstructionActors, TArray<FConstructedSlab>& ConstructedSlabs)
{

    for (AArchVizSlabActor* SlabActor : SlabConstructionActors)
    {
        if (SlabActor)
        {
            FConstructedSlab SaveSlotData;

            SaveSlotData.StartPoint = SlabActor->StartPoint;

            SaveSlotData.EndPoint = SlabActor->EndPoint;

            if (SlabActor->GetFloorMaterial())
            {
                SaveSlotData.FloorMaterial = SlabActor->GetFloorMaterial()->GetPathName();
            }
            if (SlabActor->GetRoofMaterial())
            {
                SaveSlotData.RoofMaterial = SlabActor->GetRoofMaterial()->GetPathName();
            }

            SaveSlotData.AttachedActors = GetInteriorItems(SlabActor);

            ConstructedSlabs.Add(SaveSlotData);
        }
    }
}

void UArchVizSaveLoadManager::LoadSlab(TArray<AArchVizSlabActor*>& SlabConstructionActors, const TArray<FConstructedSlab>& ConstructedSlabs)
{
    for (AArchVizSlabActor* SlabActor : SlabConstructionActors)
    {
        SlabActor->Destroy();
    }
    SlabConstructionActors.Empty();
    for (const FConstructedSlab& Slab : ConstructedSlabs)
    {
        AArchVizSlabActor* NewSlab = GetWorld()->SpawnActor<AArchVizSlabActor>();
        NewSlab->AddStartPoint(Slab.StartPoint);
        UMaterialInterface* SlabMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Slab.RoofMaterial));
        NewSlab->SetMaterial(SlabMaterial,0);
        SlabMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Slab.FloorMaterial));
        NewSlab->SetMaterial(SlabMaterial, 1);
        NewSlab->AddEndPoint(Slab.EndPoint);
        NewSlab->SetIsMoving(false);
        
        for (int i = 0; i < Slab.AttachedActors.Num(); i++)
        {
            FTransform ActorTransform = Slab.AttachedActors[i].ActorTransform;
            FActorSpawnParameters SpawnParams;

            AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>(AArchVizInteriorActor::StaticClass(), ActorTransform, SpawnParams);
            UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *Slab.AttachedActors[i].StaticMesh));
            InteriorActor->SetStaticMesh(StaticMesh, Slab.AttachedActors[i].InteriorType);
            InteriorActor->AttachToActor(NewSlab, FAttachmentTransformRules::KeepRelativeTransform);
        }
        SlabConstructionActors.Add(NewSlab);

    }
}
