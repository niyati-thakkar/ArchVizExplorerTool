// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/ArchVizInteriorManager.h"
#include "Interior/ArchVizInteriorActor.h"
#include "Game/ArchVizPlayerController.h"
#include "ConstructionActors/ArchVizSlabActor.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "Widgets/InteriorWidget.h"

void UArchVizInteriorManager::SetUp()
{
    // Initialization code
}

void UArchVizInteriorManager::End()
{
    if(InteriorWidget)
    {
	    InteriorWidget->ClearSelection();
    }
    if (CurrentActor != nullptr)
    {
        //InteriorActors.Remove(CurrentActor);
        CurrentActor->SetIsMoving(false);
        CurrentActor->UnhighlightDeselectedActor();
        CurrentActor = nullptr;
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
            HitResult = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController())->GetMouseLocation({ CurrentActor });
            PlaceActor(HitResult);
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
    UpdateUI();
}

void UArchVizInteriorManager::Start()
{
    Super::Start();
}

void UArchVizInteriorManager::AttachToCeiling(UStaticMesh* StaticMesh)
{
    
    if (CurrentActor)
    {
        CurrentActor->UnhighlightDeselectedActor();
    }
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::CeilingPlaceable);
        CurrentActor->SetIsMoving(true);
        InteriorActors.Add(InteriorActor);
    }
}

void UArchVizInteriorManager::PlaceOnFloor(UStaticMesh* StaticMesh)
{
    if (CurrentActor)
    {
        CurrentActor->UnhighlightDeselectedActor();
    }
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::FloorPlaceable);
        CurrentActor->SetIsMoving(true);
        InteriorActors.Add(InteriorActor);
    }
}

void UArchVizInteriorManager::PlaceOnWall(UStaticMesh* StaticMesh)
{
    if (CurrentActor)
    {
        CurrentActor->UnhighlightDeselectedActor();
    }
    AArchVizInteriorActor* InteriorActor = GetWorld()->SpawnActor<AArchVizInteriorActor>();
    if (InteriorActor)
    {
        CurrentActor = InteriorActor;
        CurrentActor->SetStaticMesh(StaticMesh, EInteriorItemType::WallPlaceable);
        CurrentActor->SetIsMoving(true);
        InteriorActors.Add(InteriorActor);
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
            //CurrentActor->SetActorRelativeLocation(HitResult.Location + CurrentActor->AdjustPositionForPlacement());
            //CurrentActor->SetActorRotation(HitResult.GetActor()->GetActorRotation());
            CurrentActor->SetIsMoving(false);
            CurrentActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
            CurrentActor = nullptr;
        }
    }

}

void UArchVizInteriorManager::ApplyRotation(FRotator InRotation)
{
    Super::ApplyRotation();
    if (CurrentActor && CurrentActor->IsRotateable())
    {
        CurrentActor->RotateActor();
    }
}
void UArchVizInteriorManager::UpdateUI()
{
    if(CurrentActor && InteriorWidget)
    {
	    switch(CurrentActor->ComponentType)
	    {
	    case EInteriorItemType::CeilingPlaceable:
		    {
            InteriorWidget->UpdateCeilElementSelected(CurrentActor->GetStaticMesh());
            break;
		    }
	    
        case EInteriorItemType::WallPlaceable:
        {
            InteriorWidget->UpdateWallElementSelected(CurrentActor->GetStaticMesh());
            break;
        }
        case EInteriorItemType::FloorPlaceable:
        {
            InteriorWidget->UpdateFloorElementSelected(CurrentActor->GetStaticMesh());
            break;
        }
        }
    }else
    {
        if (InteriorWidget)
        {
            InteriorWidget->ClearSelection();
        }
    }
	
}