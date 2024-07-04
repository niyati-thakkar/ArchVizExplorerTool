// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/ArchVizInteriorManager.h"
#include "Interior/ArchVizInteriorActor.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "ConstructionActors/ArchVizWallActor.h"

void UArchVizInteriorManager::SetUp()
{
    // Initialization code
}

void UArchVizInteriorManager::End()
{
    if (CurrentActor != nullptr)
    {
        CurrentActor->SetIsMoving(false);
        CurrentActor = nullptr;
        CurrentActor->UnhighlightDeselectedActor();
    }
}

void UArchVizInteriorManager::MouseClicked(FHitResult HitResult)
{

    if (CurrentActor)
    {
        CurrentActor->UnhighlightDeselectedActor();
        if (!CurrentActor->GetIsMoving())
        {
            if(CurrentActor == HitResult.GetActor())
            {
                CurrentActor->SetIsMoving(true);
            }else
            {
                CurrentActor = Cast<AArchVizInteriorActor>(HitResult.GetActor());
                if (CurrentActor)
                {
                    CurrentActor->HighlightSelectedActor();
                }
            }
           
        }else 
        {
            bool bCanPlace = false;

            switch (CurrentActor->GetComponentType())
            {
            case EInteriorItemType::WallPlaceable:
                bCanPlace = Cast<AArchVizWallActor>(HitResult.GetActor()) != nullptr;
                break;
            case EInteriorItemType::FloorPlaceable:
                bCanPlace = Cast<AArchVizSlabActor>(HitResult.GetActor()) != nullptr && HitResult.ImpactNormal.Z > 0;
                break;
            case EInteriorItemType::CeilingPlaceable:
                bCanPlace = Cast<AArchVizSlabActor>(HitResult.GetActor()) != nullptr && HitResult.ImpactNormal.Z < 0;
                break;
            default:
                break;
            }

            if (bCanPlace)
            {
                //HitResult.Location.Z = HitResult.GetActor()->GetActorLocation().Z;
                CurrentActor->SetActorLocation(HitResult.Location);
                CurrentActor->SetActorRotation(HitResult.GetActor()->GetActorRotation());
                CurrentActor->AdjustPositionForPlacement();
                CurrentActor->SetIsMoving(false);
                CurrentActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
                InteriorActors.Add(CurrentActor);
                CurrentActor = nullptr;
            }
        }
        TArray<AActor*> array;
        HitResult.GetActor()->GetAttachedActors(array);
        
    }else
    {
        CurrentActor = Cast<AArchVizInteriorActor>(HitResult.GetActor());
        if (CurrentActor)
        {
            CurrentActor->HighlightSelectedActor();
        }
    }
}

void UArchVizInteriorManager::Start()
{
    Super::Start();
}

void UArchVizInteriorManager::AttachToCeiling(UStaticMesh* StaticMesh)
{
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::CeilingPlaceable);
        CurrentActor->SetIsMoving(true);
    }
}

void UArchVizInteriorManager::PlaceOnFloor(UStaticMesh* StaticMesh)
{
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::FloorPlaceable);
        CurrentActor->SetIsMoving(true);
        
    }
}

void UArchVizInteriorManager::PlaceOnWall(UStaticMesh* StaticMesh)
{
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::WallPlaceable);
        CurrentActor->SetIsMoving(true);
    }
}

void UArchVizInteriorManager::DeleteButtonClicked()
{
    if (CurrentActor)
    {
        if (AArchVizInteriorActor* InteriorActor = Cast<AArchVizInteriorActor>(CurrentActor))
        {
            InteriorActor->Destroy();
            InteriorActors.Remove(InteriorActor);
            CurrentActor = nullptr;
        }
    }
}

void UArchVizInteriorManager::PlaceActor(FHitResult HitResult)
{
    if (CurrentActor)
    {
        bool bCanPlace = false;

        switch (CurrentActor->GetComponentType())
        {
        case EInteriorItemType::WallPlaceable:
            bCanPlace = Cast<AArchVizWallActor>(HitResult.GetActor()) != nullptr;
            break;
        case EInteriorItemType::FloorPlaceable:
            bCanPlace = Cast<AArchVizSlabActor>(HitResult.GetActor()) != nullptr && HitResult.ImpactNormal.Z > 0;
            break;
        case EInteriorItemType::CeilingPlaceable:
            bCanPlace = Cast<AArchVizSlabActor>(HitResult.GetActor()) != nullptr && HitResult.ImpactNormal.Z < 0;
            break;
        default:
            break;
        }

        if (bCanPlace)
        {
            //HitResult.Location.Z = HitResult.GetActor()->GetActorLocation().Z;
            CurrentActor->SetActorLocation(HitResult.Location);
            CurrentActor->SetActorRotation(HitResult.GetActor()->GetActorRotation());
            CurrentActor->AdjustPositionForPlacement();
            CurrentActor->SetIsMoving(false);
            CurrentActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
            InteriorActors.Add(CurrentActor);
            CurrentActor = nullptr;
        }
    }
    TArray<AActor*> array;
    CurrentActor->GetParentActor()->GetAttachedActors(array);

}

void UArchVizInteriorManager::ApplyRotation(FRotator InRotation)
{
    Super::ApplyRotation();
    if (CurrentActor && CurrentActor->IsRotateable())
    {
        CurrentActor->RotateActor();
    }
}