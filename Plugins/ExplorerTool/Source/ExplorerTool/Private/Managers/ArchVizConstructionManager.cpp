// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/ArchVizConstructionManager.h"
#include "ConstructionActors/ArchVizDoorActor.h"
#include "ConstructionActors/ArchVizDoorWallActor.h"
#include "Widgets/ConstructionWidget.h"

// Helper function to destroy attached actors
void DestroyAttachedActors(AActor* Actor)
{
    TArray<AActor*> AttachedActors;
    Actor->GetAttachedActors(AttachedActors);

    for (AActor* AttachedActor : AttachedActors)
    {
        if (AttachedActor)
        {
            AttachedActor->Destroy(); 
        }
    }
}

void UArchVizConstructionManager::DeleteButtonClicked()
{
    if (!CurrentActor)
        return;

    if (AArchVizWallActor* WallActor = Cast<AArchVizWallActor>(CurrentActor))
    {
        DestroyAttachedActors(WallActor);
        WallActor->Destroy();
        WallActors.Remove(WallActor);
    }
    else if (AArchVizSlabActor* SlabActor = Cast<AArchVizSlabActor>(CurrentActor))
    {
        DestroyAttachedActors(SlabActor);
        SlabActor->Destroy();
        SlabActor->SetMaterial(nullptr, 0); // Clear floor material
        SlabActor->SetMaterial(nullptr, 1); // Clear roof material
        SlabActors.Remove(SlabActor);
    }
    else if (AArchVizDoorActor* DoorActor = Cast<AArchVizDoorActor>(CurrentActor))
    {
        if (AArchVizDoorWallActor* DoorWallActor = Cast<AArchVizDoorWallActor>(DoorActor->GetAttachParentActor()))
        {
            if (AArchVizWallActor* Actor = Cast<AArchVizWallActor>(DoorWallActor->GetAttachParentActor()))
            {
                Actor->ReplaceDoorWithWall(DoorWallActor);
            }

            DestroyAttachedActors(DoorWallActor);
            DoorWallActor->Destroy();
        }
    }

    // Destroy the current actor and clear the reference
    CurrentActor->Destroy();
    CurrentActor = nullptr;
}

void UArchVizConstructionManager::SetUp()
{
    CurrentActor = nullptr;
}

void UArchVizConstructionManager::End()
{
    if(ConstructionWidget)
    {
        ConstructionWidget->ClearSelected();
    }
    if (!CurrentActor)
        return;

    CurrentActor->UnhighlightDeselectedActor();
    if (CurrentActor->GetIsMoving())
    {
        CurrentActor->Destroy();
        CurrentActor->SetIsMoving(false);
    }
    CurrentActor = nullptr;
}

void UArchVizConstructionManager::MouseClicked(FHitResult HitResult)
{
    if (CurrentActor)
    {
        CurrentActor->UnhighlightDeselectedActor();
    }

    if (CurrentConstructionType == EConstructionTypes::DoorAddition)
    {
        HandleDoorAddition(HitResult);
    }
    else
    {
        HandleOtherConstructionTypes(HitResult);
    }
}

void UArchVizConstructionManager::HandleDoorAddition(FHitResult HitResult)
{
    if (!IsValid(CurrentActor))
    {
        SelectCurrentActor(HitResult);
        return;
    }

    if (CurrentActor->GetIsMoving())
    {
        HandleDoorPlacement(HitResult);
    }
    else
    {
        HandleDoorSelectionOrMove(HitResult);
    }
}

void UArchVizConstructionManager::HandleDoorPlacement(FHitResult HitResult)
{
    if (AArchVizDoorActor* DActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
    {
        if (DActor->GetWallActor())
        {
            DActor->GetWallActor()->ReplaceWallWithDoor(HitResult.Location);
            DActor->Destroy();
            CurrentActor = nullptr;
        }
    }
}

void UArchVizConstructionManager::HandleDoorSelectionOrMove(FHitResult HitResult)
{
    if (CurrentActor == HitResult.GetActor())
    {
        if (AArchVizDoorActor* DActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
        {
            if (IsValid(DActor->GetAttachParentActor()) && IsValid(DActor->GetAttachParentActor()->GetAttachParentActor()))
            {
                Cast<AArchVizWallActor>(DActor->GetAttachParentActor()->GetAttachParentActor())->ReplaceDoorWithWall(Cast<AArchVizDoorWallActor>(DActor->GetAttachParentActor()));
                DActor->SetIsMoving(true);
            }
        }
    }
    else
    {
        SelectCurrentActor(HitResult);
    }
}

void UArchVizConstructionManager::HandleOtherConstructionTypes(FHitResult HitResult)
{
    if (CurrentActor)
    {
        HandleExistingActor(HitResult);
    }
    else
    {
        HandleNewActor(HitResult);
    }
}

void UArchVizConstructionManager::HandleExistingActor(FHitResult HitResult)
{
    CurrentActor->UnhighlightDeselectedActor();

    if (!CurrentActor->GetIsConstructed())
    {
        CurrentActor->SetIsConstructed(true);
        CurrentActor->SetIsMoving(false);
        CurrentActor = nullptr;
    }
    else if (CurrentActor->GetIsMoving())
    {
        CurrentActor->SetIsMoving(false);
        CurrentActor = nullptr;
    }
    else
    {
        HandleActorSelection(HitResult);
    }
    
    
}

void UArchVizConstructionManager::HandleActorSelection(FHitResult HitResult)
{
    if (CurrentActor == HitResult.GetActor())
    {
        CurrentActor->SetIsMoving(true);
    }
    else if (AArchVizDoorWallActor* InActor = Cast<AArchVizDoorWallActor>(HitResult.GetActor()))
    {
        if (CurrentActor == InActor->GetAttachParentActor())
        {
            CurrentActor->SetIsMoving(true);
        }
    }
    else if (AArchVizDoorActor* ThisActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
    {
        if (ThisActor->GetAttachParentActor())
        {
            if (CurrentActor == ThisActor->GetAttachParentActor()->GetAttachParentActor())
            {
                CurrentActor->SetIsMoving(true);
            }
        }
    }
    else
    {
        SelectConstructionTypeActor(HitResult);
    }
}

void UArchVizConstructionManager::HandleNewActor(FHitResult HitResult)
{
    if (CurrentConstructionType == EConstructionTypes::WallGeneration)
    {
        HandleNewWallActor(HitResult);
    }
    else if (CurrentConstructionType == EConstructionTypes::SlabGeneration)
    {
        HandleNewSlabActor(HitResult);
    }
}

void UArchVizConstructionManager::HandleNewWallActor(FHitResult HitResult)
{
    if (AArchVizWallActor* Actor = Cast<AArchVizWallActor>(HitResult.GetActor()))
    {
        CurrentActor = Actor;
        CurrentActor->HighlightSelectedActor();
    }
    else if (AArchVizDoorWallActor* InActor = Cast<AArchVizDoorWallActor>(HitResult.GetActor()))
    {
        CurrentActor = Cast<AArchVizWallActor>(InActor->GetAttachParentActor());
        if (CurrentActor)
        {
            CurrentActor->HighlightSelectedActor();
        }
    }
    else if (AArchVizDoorActor* ThisActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
    {
        CurrentActor = Cast<AArchVizWallActor>(ThisActor->GetAttachParentActor()->GetAttachParentActor());
        if (CurrentActor)
        {
            CurrentActor->HighlightSelectedActor();
        }
    }
    else
    {
        CurrentActor = GetWorld()->SpawnActor<AArchVizWallActor>();
        Cast<AArchVizWallActor>(CurrentActor)->AddStartPoint(HitResult.Location);
        WallActors.Add(Cast<AArchVizWallActor>(CurrentActor));
    }
    if(CurrentActor)
    {
        UpdateUI();
    }
}

void UArchVizConstructionManager::HandleNewSlabActor(FHitResult HitResult)
{
    if (AArchVizSlabActor* Actor = Cast<AArchVizSlabActor>(HitResult.GetActor()))
    {
        CurrentActor = Actor;
        CurrentActor->HighlightSelectedActor();
    }
    else
    {
        CurrentActor = GetWorld()->SpawnActor<AArchVizSlabActor>();
        Cast<AArchVizSlabActor>(CurrentActor)->AddStartPoint(HitResult.Location);
        SlabActors.Add(Cast<AArchVizSlabActor>(CurrentActor));
    }
    if (CurrentActor)
    {
        UpdateUI();
    }
}

void UArchVizConstructionManager::SelectCurrentActor(FHitResult HitResult)
{
    CurrentActor = Cast<AArchVizDoorActor>(HitResult.GetActor());
    if (CurrentActor)
    {
        CurrentActor->HighlightSelectedActor();
    }
}

void UArchVizConstructionManager::SelectConstructionTypeActor(FHitResult HitResult)
{
    if (CurrentConstructionType == EConstructionTypes::WallGeneration)
    {
        HandleNewWallActor(HitResult);
    }
    else if (CurrentConstructionType == EConstructionTypes::SlabGeneration)
    {
        HandleNewSlabActor(HitResult);
    }
}

void UArchVizConstructionManager::ChangeConstructionType(EConstructionTypes ConstructionType)
{
    End();
    if (CurrentConstructionType == EConstructionTypes::DoorAddition && CurrentConstructionType == ConstructionType)
    {
        CurrentActor = GetWorld()->SpawnActor<AArchVizDoorActor>();
        CurrentActor->SetIsMoving(true);
    }
    
    CurrentConstructionType = ConstructionType;
}

void UArchVizConstructionManager::Start()
{
    Super::Start();
}

void UArchVizConstructionManager::ChangeWallMaterial(UMaterialInterface* Material)
{
    if (CurrentConstructionType == EConstructionTypes::WallGeneration && CurrentActor)
    {
        if (AArchVizWallActor* WallActor = Cast<AArchVizWallActor>(CurrentActor))
        {
            WallActor->SetMaterial(Material);
        }
    }
}

void UArchVizConstructionManager::ChangeFloorMaterial(UMaterialInterface* Material)
{
    if (CurrentConstructionType == EConstructionTypes::SlabGeneration && CurrentActor)
    {
        if (AArchVizSlabActor* SlabActor = Cast<AArchVizSlabActor>(CurrentActor))
        {
            SlabActor->SetMaterial(Material, 1);
        }
    }
}

void UArchVizConstructionManager::ChangeRoofMaterial(UMaterialInterface* Material)
{
    if (CurrentConstructionType == EConstructionTypes::SlabGeneration && CurrentActor)
    {
        if (AArchVizSlabActor* SlabActor = Cast<AArchVizSlabActor>(CurrentActor))
        {
            SlabActor->SetMaterial(Material, 0);
        }
    }
}

void UArchVizConstructionManager::ApplyRotation(FRotator InRotation)
{
    Super::ApplyRotation();
    if (CurrentActor && CurrentActor->IsRotateable())
    {
        CurrentActor->RotateActor();
    }
}
void UArchVizConstructionManager::UpdateUI()
{
    if (CurrentConstructionType == EConstructionTypes::WallGeneration)
    {
        if(AArchVizWallActor* actor = Cast<AArchVizWallActor>(CurrentActor))
        {
            ConstructionWidget->UpdatePropertiesSelected(actor->GetMaterial());
        }
			
    }else
    {
        if (AArchVizSlabActor* actor = Cast<AArchVizSlabActor>(CurrentActor))
        {
			ConstructionWidget->UpdatePropertiesSelected(actor->GetFloorMaterial(), actor->GetRoofMaterial());
        }
    }
}