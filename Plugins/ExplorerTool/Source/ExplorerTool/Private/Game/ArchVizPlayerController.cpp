
#include "Game/ArchVizPlayerController.h"
#include "SaveLoad/ArchVizGameInstanceSubsystem.h"
#include "SaveLoad/ArchVizSaveTool.h"
#include "EngineUtils.h"
#include "Tools/UEdMode.h"

void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    AddMappings();
}

void AArchVizPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (FollowArrowActorClass) {
        FollowArrowActor = GetWorld()->SpawnActor<AArchVizFollowArrowActor>(FollowArrowActorClass);
        FollowArrowActor->SetActorLocation(FVector(0, 0, 500));
        FollowArrowActor->SetActorScale3D(FVector(10));
    }
    RoadManager->SetUp();
    InteriorManager->SetUp();
    ConstructionManager->SetUp();
    SaveLoadManager->SetUp();
}

AArchVizPlayerController::AArchVizPlayerController()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    RoadManager = CreateDefaultSubobject<UArchVizRoadManager>(TEXT("RoadManager"));
    InteriorManager = CreateDefaultSubobject<UArchVizInteriorManager>(TEXT("InteriorManager"));
    ConstructionManager = CreateDefaultSubobject<UArchVizConstructionManager>(TEXT("ConstructionManager"));
    SaveLoadManager = CreateDefaultSubobject<UArchVizSaveLoadManager>(TEXT("SaveLoadManager"));
}

void AArchVizPlayerController::AddMappings()
{
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        UInputMappingContext* IMC = NewObject<UInputMappingContext>();

        UInputAction* LeftClickAction = NewObject<UInputAction>();
        LeftClickAction->ValueType = EInputActionValueType::Axis3D;

        IMC->MapKey(LeftClickAction, EKeys::LeftMouseButton);
        EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::GetMouseClickLocation);

        UInputAction* SaveAction = NewObject<UInputAction>();
        UInputAction* LoadAction = NewObject<UInputAction>();

        IMC->MapKey(SaveAction, EKeys::S);
        IMC->MapKey(LoadAction, EKeys::L);

        EIC->BindAction(SaveAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::Save);
        EIC->BindAction(LoadAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::Load);

        const auto LocalPlayer = GetLocalPlayer();
        if (LocalPlayer)
        {
            if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                SubSystem->AddMappingContext(IMC, 0);
            }
        }

        bShowMouseCursor = true;
        DefaultMouseCursor = EMouseCursor::Crosshairs;
    }
}

void AArchVizPlayerController::ArrowSelected()
{
    bIsArrowSelected = !bIsArrowSelected;

    if (GridActor && GridActor->bIsGridVisible)
    {
        GridActor->bIsGridVisible = false;
        GridActor->HideGrid();
    /*    FollowArrowActor->Destroy();
        FollowArrowActor = nullptr;*/
    }
}

void AArchVizPlayerController::HandleArrowSelection(const FHitResult& HitResult)
{
    if (!GridActor)
    {
        GridActor = GetWorld()->SpawnActor<AArchVizGridActor>();
    }

    if (GridActor)
    {
        FollowArrowActor->SetActorHiddenInGame(true);
        if (GridActor->bIsGridVisible && !FollowArrowActor->GetRootComponent()->bHiddenInGame)
        {
            //GridActor->GenerateWall(HitResult.Location);
        }
        else
        {
            // GridActor->SetGridPosition(FVector(0,0,100));
            GridActor->ShowGrid();
            GridActor->bIsGridVisible = true;
        }
    }
   /* FollowArrowActor = GetWorld()->SpawnActor<AArchVizFollowArrowActor>(FollowArrowActorClass);
    UpdateArrowPosition();*/
}

void AArchVizPlayerController::HighlightSelectedActor() {

    if (!CurrentSelectedActor) return;

    TSet<UActorComponent*> ActorComponents = CurrentSelectedActor->GetComponents();

    for (auto& ActorComponent : ActorComponents) {

        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {

            Component->SetRenderCustomDepth(true);

            Component->CustomDepthStencilValue = 2;

        }

    }

}

void AArchVizPlayerController::UnhighlightDeselectedActor() {

    if (!CurrentSelectedActor) return;

    TSet<UActorComponent*> ActorComponents = CurrentSelectedActor->GetComponents();

    for (auto& ActorComponent : ActorComponents) {

        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {

            Component->SetRenderCustomDepth(false);

        }

    }

}
void AArchVizPlayerController::GetMouseClickLocation(const FInputActionValue& InputAction)
{
    float MouseX, MouseY;
    FVector WorldLocation, WorldDirection;
    FHitResult HitResult;
    if (GetMousePosition(MouseX, MouseY))
    {
        if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
        {
            
            FVector TraceEnd = WorldLocation + (WorldDirection * 50000.f);

            FCollisionQueryParams QueryParams;
            QueryParams.bTraceComplex = true;
            QueryParams.AddIgnoredActor(GetPawn());

            if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
            {
                AActor* HitActor = HitResult.GetActor();
                if (HitActor && !HitActor->IsA<AArchVizActor>() && GetManager())
                {
                    GetManager()->FloorHit(HitResult);
                }
                else if (HitActor && HitActor->IsA<AArchVizActor>())
                {
                    UnhighlightDeselectedActor();
                    if(AArchVizRoadActor* SelectedActor = Cast<AArchVizRoadActor>(HitActor))
                    {
                        ChangeMode(EArchVizMode::RoadMode);
                        GetManager()->ActorHit(SelectedActor);
                        CurrentSelectedActor = SelectedActor;
                    }
                    
                    HighlightSelectedActor();
                }
                /*if (bIsArrowSelected)
                {
                    HandleArrowSelection(HitResult);
                }*/
            }
        }
    }
}

UArchVizManager* AArchVizPlayerController::GetManager()
{
    if(CurrentMode == EArchVizMode::RoadMode)
    {
        return RoadManager;
    }
    if(CurrentMode == EArchVizMode::ConstructionMode)
    {
        return ConstructionManager;
    }
    if(CurrentMode == EArchVizMode::InteriorMode)
    {
        return InteriorManager;
    }
    if(CurrentMode == EArchVizMode::SaveLoadMode)
    {
        return SaveLoadManager;
    }
    return nullptr;
}





void AArchVizPlayerController::UpdateArrowPosition()
{
    // Get mouse position
    float MouseX, MouseY;
    GetMousePosition(MouseX, MouseY);

    // Deproject mouse position to world
    FVector WorldLocation, WorldDirection;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    // Calculate trace end
    FVector TraceEnd = WorldLocation + WorldDirection * 10000.f;

    // Perform a trace to find the ground location
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    CollisionParams.AddIgnoredActor(GetPawn());

    if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, CollisionParams))
    {
        AActor* HitActor = HitResult.GetActor();

        // Check if the hit actor is a grid actor
        if (HitActor && HitActor->IsA<AArchVizGridActor>())
        {
            GridActor = Cast<AArchVizGridActor>(HitActor);
            FVector GridCenter = GridActor->GetActorLocation();
            FVector GridOffset = FVector(GridActor->GridSize * GridActor->GridCellSize * 0.5f, GridActor->GridSize * GridActor->GridCellSize * 0.5f, 0.0f);
            FVector GridStart = GridCenter - GridOffset;

            int32 XIndex = FMath::FloorToInt((HitResult.Location.X - GridStart.X) / GridActor->GridCellSize);
            int32 YIndex = FMath::FloorToInt((HitResult.Location.Y - GridStart.Y) / GridActor->GridCellSize);

            // Ensure the indices are within grid bounds
            if (XIndex >= 0 && XIndex < GridActor->GridSize && YIndex >= 0 && YIndex < GridActor->GridSize)
            {
                FVector GridPointLocation = GridStart + FVector((XIndex + 0.5f) * GridActor->GridCellSize, (YIndex + 0.5f) * GridActor->GridCellSize, 100.0f);
                FollowArrowActor->SetActorLocation(GridPointLocation);
                LastHitLocation = GridPointLocation;
                FollowArrowActor->SetActorHiddenInGame(false);
            }
            else
            {
                FollowArrowActor->SetActorHiddenInGame(true);
            }
        }
        else
        {
            FollowArrowActor->SetActorHiddenInGame(true);
        }
    }
    else
    {
        FollowArrowActor->SetActorHiddenInGame(true);
    }
}


void AArchVizPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
   if (bIsArrowSelected && FollowArrowActor)
    {
       UpdateArrowPosition();
    }
}

void AArchVizPlayerController::ChangeMode(EArchVizMode SetMode)
{
    if(GetManager())
    {
        GetManager()->End();
    }
    if(SetMode != CurrentMode)
    {
        CurrentSelectedActor = nullptr;
    }
    CurrentMode = SetMode;
    if(GetManager())
    {
        GetManager()->Start();
    }
}

EArchVizMode AArchVizPlayerController::GetCurrentMode()
{
    return CurrentMode;
}

void AArchVizPlayerController::Save()
{
    SaveLoadManager->SaveRoad(RoadManager->RoadConstructionActors);
}
void AArchVizPlayerController::Load()
{
    SaveLoadManager->LoadRoad(RoadManager->RoadConstructionActors);
}