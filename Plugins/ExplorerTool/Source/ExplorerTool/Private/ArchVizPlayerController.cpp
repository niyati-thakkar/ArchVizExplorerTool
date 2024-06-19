// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizPlayerController.h"

#include "ArchVizGameInstanceSubsystem.h"
#include "ArchVizSaveTool.h"
#include "EngineUtils.h"

void AArchVizPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	AddRoadConstructionMappings();


}

void AArchVizPlayerController::BeginPlay()
{
	Super::BeginPlay();
	RoadConstructionActors.Add(GetWorld()->SpawnActor<ARoadConstructionActor>());
	if(TaskBarClass)
	{
		TaskBar = CreateWidget<UTaskBar>(this, TaskBarClass);
		if (TaskBar && TaskBarDataAsset) {
			
			TaskBar->TaskBarData = TaskBarDataAsset;
			TaskBar->InitializeWidget();
			TaskBar->AddToViewport();
			TaskBar->ButtonClicked.BindUFunction(this, "WidgetSelected");
			
		}
	}
}

AArchVizPlayerController::AArchVizPlayerController()
{
	SaveSlotName = "GameSlotOne";

	//static ConstructorHelpers::FObjectFinder<UTaskBarDataAsset> TaskBarDataAssetObj(TEXT("/ExplorerTool/DA_TaskBar.DA_TaskBar"));


}

void AArchVizPlayerController::AddRoadConstructionMappings()
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		UInputMappingContext* IMC = NewObject<UInputMappingContext>();


		UInputAction* LeftClickAction = NewObject<UInputAction>();
		LeftClickAction->ValueType = EInputActionValueType::Axis3D;

		IMC->MapKey(LeftClickAction, EKeys::LeftMouseButton);

		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::GetMouseClickLocation);

		UInputAction* SaveAction = NewObject<UInputAction>();
		UInputAction* LoadAction = NewObject<UInputAction>();

		IMC->MapKey(SaveAction, EKeys::S);
		IMC->MapKey(LoadAction, EKeys::L);


		EIC->BindAction(SaveAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::SaveSlot);
		EIC->BindAction(LoadAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::LoadSlot);

		const auto LocalPlayer = GetLocalPlayer();

		if (LocalPlayer)
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				//SubSystem->ClearAllMappings();

				// Add the input mapping context to the local player subsystem
				SubSystem->AddMappingContext(IMC, 0);
			}
		}
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Crosshairs;

	}
}
void AArchVizPlayerController::GetMouseClickLocation(const FInputActionValue& InputAction) {
	float MouseX, MouseY;
	FVector WorldLocation, WorldDirection;
	if (GetMousePosition(MouseX, MouseY)) {
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
			FHitResult HitResult;
			FVector TraceEnd = WorldLocation + (WorldDirection * 50000.f);

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(GetPawn());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams)) {
				if (HitResult.GetActor()) {


					GenerateRoadPoint(HitResult);
				}



			}
		}
	}
}

void AArchVizPlayerController::GenerateRoadPoint(FHitResult HitResult)
{

	RoadConstructionActors[0]->AddRoadPoint(HitResult.Location);
}

void AArchVizPlayerController::SaveSlot()
{
	if (UArchVizGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
	{
		FString CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
		if (CurrentSaveSlotName.IsEmpty())
		{
			Subsystem->CreateSaveGameSlot("GameSlotOne");
			CurrentSaveSlotName = Subsystem->GetCurrentSaveGameSlotName();
		}

		// Ensure the slot is valid before proceeding
		if (!CurrentSaveSlotName.IsEmpty())
		{
			FSaveSlotElement& SaveSlot = Subsystem->GetCurrentSaveGameSlot()->GameSlots.FindOrAdd(CurrentSaveSlotName);
			SaveSlot.RoadElements.Empty(); // Clear previous data before saving new data

			for (ARoadConstructionActor* RoadActor : RoadConstructionActors)
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
void AArchVizPlayerController::LoadSlot()
{
	if (UArchVizGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UArchVizGameInstanceSubsystem>())
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

				// Clear the current road construction actors list
				RoadConstructionActors.Empty();

				// Recreate road actors from saved data
				for (const FConstructedRoad& SaveData : SaveSlot->RoadElements)
				{
					ARoadConstructionActor* NewRoadActor = GetWorld()->SpawnActor<ARoadConstructionActor>();
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
void AArchVizPlayerController::WidgetSelected(FText SelectionName)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("got the message"));
}