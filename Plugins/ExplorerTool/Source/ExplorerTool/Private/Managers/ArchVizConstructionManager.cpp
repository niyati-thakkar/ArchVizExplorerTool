// Fill out your copyright notice in the Description page of Project Settings.



#include "Managers/ArchVizConstructionManager.h"

#include "ConstructionActors/ArchVizDoorActor.h"
#include "ConstructionActors/ArchVizDoorWallActor.h"


void UArchVizConstructionManager::DeleteButtonClicked()
{
	if (CurrentActor)
	{
		// Helper function to destroy attached actors
		auto DestroyAttachedActors = [](AActor* Actor)
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
			};

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
}



void UArchVizConstructionManager::SetUp()
{
	CurrentActor = nullptr;
}

void UArchVizConstructionManager::End()
{
	if(CurrentActor != nullptr)
	{
		CurrentActor->UnhighlightDeselectedActor();
		if(CurrentActor->GetIsMoving())
		{
			CurrentActor->Destroy();
			CurrentActor->SetIsMoving(false);
			
		}
		CurrentActor = nullptr;
		
	}
	
}

void UArchVizConstructionManager::MouseClicked(FHitResult HitResult)
{
	if(CurrentActor)
	{
		CurrentActor->UnhighlightDeselectedActor();
	}
	if (CurrentConstructionType == EConstructionTypes::DoorAddition)
	{
		if(IsValid(CurrentActor))
		{
			if(CurrentActor->GetIsMoving())
			{
				if (AArchVizDoorActor* DActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
				{
					if (DActor->GetWallActor()) {
						DActor->GetWallActor()->ReplaceWallWithDoor(HitResult.Location);
						DActor->Destroy();
						DActor = nullptr;
						CurrentActor = nullptr;
					}
				}
			}else
			{
				if(CurrentActor == HitResult.GetActor())
				{
					if (AArchVizDoorActor* DActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
					{
						if(IsValid(DActor->GetAttachParentActor()) && IsValid(DActor->GetAttachParentActor()->GetAttachParentActor()))
						{
							Cast<AArchVizWallActor>(DActor->GetAttachParentActor()->GetAttachParentActor())->ReplaceDoorWithWall(Cast<AArchVizDoorWallActor>(DActor->GetAttachParentActor()));
							DActor->SetIsMoving(true);
						}
						
					}
				}
				CurrentActor = Cast<AArchVizDoorActor>(HitResult.GetActor());
				if(CurrentActor)
				{
					CurrentActor->HighlightSelectedActor();
				}
			}
		}else
		{
			CurrentActor = Cast<AArchVizDoorActor>(HitResult.GetActor());
			if (CurrentActor)
			{
				CurrentActor->HighlightSelectedActor();
			}
		}
	}else
	{
		if (CurrentActor)
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
			else {
				if (CurrentActor == HitResult.GetActor()) {
					CurrentActor->SetIsMoving(true);
				}
				else if (AArchVizDoorWallActor* _InActor = Cast<AArchVizDoorWallActor>(HitResult.GetActor()))
				{
					if(CurrentActor == _InActor->GetAttachParentActor())
					{
						CurrentActor->SetIsMoving(true);
					}
					
				}
				else if (AArchVizDoorActor* _ThisActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
				{
					if(_ThisActor->GetAttachParentActor())
					{
						if (CurrentActor == _ThisActor->GetAttachParentActor()->GetAttachParentActor())
						{
							CurrentActor->SetIsMoving(true);
						}
					}
					
				}
				else {
					if (CurrentConstructionType == EConstructionTypes::WallGeneration) {
						if (AArchVizWallActor* Actor = Cast<AArchVizWallActor>(HitResult.GetActor()))
						{
							CurrentActor = Actor;
							CurrentActor->HighlightSelectedActor();
						}else if(AArchVizDoorWallActor* InActor = Cast<AArchVizDoorWallActor>(HitResult.GetActor()))
						{
							CurrentActor = Cast<AArchVizWallActor>(InActor->GetAttachParentActor());
							if(CurrentActor)
							{
								CurrentActor->HighlightSelectedActor();
							}
						}else if(AArchVizDoorActor* ThisActor = Cast<AArchVizDoorActor>(HitResult.GetActor()))
						{
							CurrentActor = Cast<AArchVizWallActor>(ThisActor->GetAttachParentActor()->GetAttachParentActor());
							if (CurrentActor)
							{
								CurrentActor->HighlightSelectedActor();
							}
						}
					}
					else if (CurrentConstructionType == EConstructionTypes::WallGeneration) {
						if (AArchVizSlabActor* Actor = Cast<AArchVizSlabActor>(HitResult.GetActor()))
						{
							CurrentActor = Actor;
							CurrentActor->HighlightSelectedActor();
						}
					}
				}
			}
		}
		else
		{
			if (CurrentConstructionType == EConstructionTypes::WallGeneration)
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

				


			}
			else if (CurrentConstructionType == EConstructionTypes::SlabGeneration)
			{
				if (AArchVizSlabActor* Actor = Cast<AArchVizSlabActor>(HitResult.GetActor()))
				{
					CurrentActor = Actor;
					CurrentActor->HighlightSelectedActor();
				}else
				{
					CurrentActor = GetWorld()->SpawnActor<AArchVizSlabActor>();
					Cast<AArchVizSlabActor>(CurrentActor)->AddStartPoint(HitResult.Location);
					SlabActors.Add(Cast<AArchVizSlabActor>(CurrentActor));
				}
				

			}
		}
	}
	
}


void UArchVizConstructionManager::ChangeConstructionType(EConstructionTypes ConstructionType)
{
	End();
	if(CurrentConstructionType == ConstructionType)
	{
		CurrentConstructionType = EConstructionTypes::None;
		CurrentActor = nullptr;
		return;
	}
	else if(CurrentConstructionType == EConstructionTypes::DoorAddition)
	{
		if(AArchVizDoorActor* Actor = Cast<AArchVizDoorActor>(CurrentActor))
		{
			CurrentActor->UnhighlightDeselectedActor();
			Actor = nullptr;
			CurrentActor = nullptr;
		}
		
	}
	CurrentConstructionType = ConstructionType;
	if(ConstructionType == EConstructionTypes::DoorAddition)
	{
		CurrentActor = GetWorld()->SpawnActor<AArchVizDoorActor>();
		CurrentActor->SetIsMoving(true);
	}
	
}
void UArchVizConstructionManager::Start()
{
	Super::Start();
}

void UArchVizConstructionManager::ChangeWallMaterial(UMaterialInterface* Material)
{
	if(CurrentConstructionType == EConstructionTypes::WallGeneration)
	{
		if(CurrentActor)
		{
			if(AArchVizWallActor* WallActor = Cast<AArchVizWallActor>(CurrentActor))
			{
				WallActor->SetMaterial(Material);
			}
			
		}
	}
}

void UArchVizConstructionManager::ChangeFloorMaterial(UMaterialInterface* Material)
{
	if (CurrentConstructionType == EConstructionTypes::SlabGeneration)
	{
		if (CurrentActor)
		{
			if (AArchVizSlabActor* SlabActor = Cast<AArchVizSlabActor>(CurrentActor))
			{
				SlabActor->SetMaterial(Material, 1);
			}

		}
	}
}
void UArchVizConstructionManager::ChangeRoofMaterial(UMaterialInterface* Material)
{
	if (CurrentConstructionType == EConstructionTypes::SlabGeneration)
	{
		if (CurrentActor)
		{
			if (AArchVizSlabActor* SlabActor = Cast<AArchVizSlabActor>(CurrentActor))
			{
				SlabActor->SetMaterial(Material, 0);
			}

		}
	}
}

void UArchVizConstructionManager::ApplyRotation(FRotator InRotation)
{
	Super::ApplyRotation();
	if(CurrentActor && CurrentActor->IsRotateable())
	{
		CurrentActor->RotateActor();
	}
}
