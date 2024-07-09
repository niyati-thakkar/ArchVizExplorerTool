// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ArchVizExteriorManager.h"

#include "ArchVizEnums.h"
#include "Game/ArchVizPlayerController.h"

void UArchVizExteriorManager::ChangeStairsWidth(float GetValue)
{
    if (IsValid(CurrentSelectedActor))
    {
        CurrentSelectedActor->SetWidth(GetValue);
    }
    
}

void UArchVizExteriorManager::ChangeStairsLength(float GetValue)
{
    if (IsValid(CurrentSelectedActor))
    {
        CurrentSelectedActor->SetLength(GetValue);
    }
    
}

void UArchVizExteriorManager::ChangeStairsDepth(float GetValue)
{
    if (IsValid(CurrentSelectedActor))
    {
        CurrentSelectedActor->SetDepth(GetValue);
    }
    
}

void UArchVizExteriorManager::ChangeNumberOfStairs(float GetValue)
{
    if (IsValid(CurrentSelectedActor))
    {
        CurrentSelectedActor->SetNumberOfStairs(GetValue);
    }
    
}

void UArchVizExteriorManager::ChangeStairsType(EStairType StairType)
{
    if (!IsValid(CurrentSelectedActor))
    {
        CurrentSelectedActor = GetWorld()->SpawnActor<AStaircaseActor>();
        SpawnedStairs.Add(CurrentSelectedActor);
        CurrentSelectedActor->SetIsMoving(true);
    }
    CurrentSelectedActor->SetStairType(StairType);
    
    
    
}
void UArchVizExteriorManager::MouseClicked(FHitResult HitResult)
{

    if (CurrentSelectedActor)
    {
        
        CurrentSelectedActor->UnhighlightDeselectedActor();
        if (!CurrentSelectedActor->GetIsMoving())
        {
            if (CurrentSelectedActor == HitResult.GetActor())
            {
                CurrentSelectedActor->SetIsMoving(true);
            }
            else
            {
                HitResult = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController())->GetMouseLocation({ CurrentSelectedActor });
                CurrentSelectedActor = Cast<AStaircaseActor>(HitResult.GetActor());
                if (CurrentSelectedActor)
                {
                    CurrentSelectedActor->HighlightSelectedActor();
                }
            }

        }
        else
        {

            //HitResult.Location.Z = HitResult.GetActor()->GetActorLocation().Z;
            //CurrentSelectedActor->SetActorLocation(HitResult.Location);

            CurrentSelectedActor->SetIsMoving(false);
            CurrentSelectedActor = nullptr;
        }
    }
    else
    {
        CurrentSelectedActor = Cast<AStaircaseActor>(HitResult.GetActor());
        if (CurrentSelectedActor)
        {
            
            CurrentSelectedActor->HighlightSelectedActor();
        }
    }
}

void UArchVizExteriorManager::End()
{
    CurrentSelectedActor = nullptr;
}
void UArchVizExteriorManager::SetUp()
{
    CurrentSelectedActor = nullptr;
}

void UArchVizExteriorManager::DeleteStairs()
{
    if(CurrentSelectedActor)
    {
        CurrentSelectedActor->DestroyStairs();
        CurrentSelectedActor->Destroy();
        CurrentSelectedActor = nullptr;
    }
}

void UArchVizExteriorManager::ApplyRotation(FRotator InRotation)
{
    CurrentSelectedActor->RotateActor(InRotation);
}
