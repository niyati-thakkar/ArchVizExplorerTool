
#include "Game/ArchVizPlayerController.h"
#include "SaveLoad/ArchVizGameInstanceSubsystem.h"
#include "SaveLoad/ArchVizSaveTool.h"
#include "EngineUtils.h"
#include "Tools/UEdMode.h"
#include "Widgets/ArchVizMasterWidget.h"

void AArchVizPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    AddMappings();
}

void AArchVizPlayerController::BeginPlay()
{
    Super::BeginPlay();
    RoadManager = NewObject<UArchVizRoadManager>(this);
    InteriorManager = NewObject<UArchVizInteriorManager>(this);
    ConstructionManager = NewObject<UArchVizConstructionManager>(this);
    SaveLoadManager = NewObject<UArchVizSaveLoadManager>(this);
    ExteriorManager = NewObject<UArchVizExteriorManager>(this);
    RoadManager->SetUp();
    InteriorManager->SetUp();
    ConstructionManager->SetUp();
    SaveLoadManager->SetUp();
    ExteriorManager->SetUp();

    if(UArchVizMasterWidget* DisplayWidget = Cast<UArchVizMasterWidget>(MasterWidget))
    {
        RoadManager->RoadWidget = DisplayWidget->RoadModeWidget;
    }
    //RoadManager->RoadWidget = MasterWidget->RoadWidget;
}

void AArchVizPlayerController::Save()
{
    SaveLoadManager->SaveSlot(RoadManager->RoadConstructionActors, ConstructionManager->WallActors, ConstructionManager->SlabActors);
}

AArchVizPlayerController::AArchVizPlayerController()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    
}

void AArchVizPlayerController::AddMappings()
{
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        UInputMappingContext* IMC = NewObject<UInputMappingContext>();

        UInputAction* LeftClickAction = NewObject<UInputAction>();
        LeftClickAction->ValueType = EInputActionValueType::Axis3D;

        UInputAction* RotationAction = NewObject<UInputAction>();

        IMC->MapKey(LeftClickAction, EKeys::LeftMouseButton);
        EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::GetMouseClickLocation);


        IMC->MapKey(RotationAction, EKeys::R);
        EIC->BindAction(RotationAction, ETriggerEvent::Completed, this, &AArchVizPlayerController::ApplyRotation);
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


void AArchVizPlayerController::GetMouseClickLocation(const FInputActionValue& InputAction)
{
    FHitResult HitResult = GetMouseLocation({});

    if(UArchVizManager* manager = GetManager())
    {
        manager->MouseClicked(HitResult);
    }
}
void AArchVizPlayerController::ApplyRotation(const FInputActionValue& InputAction)
{
    if(GetManager())
    {
        GetManager()->ApplyRotation();
    }
}
FHitResult AArchVizPlayerController::GetMouseLocation(const TArray<AActor*>& IgnoredActors)
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
            QueryParams.AddIgnoredActors(IgnoredActors);

            if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
            {
                return HitResult;

            }
        }
    }
    return FHitResult();
}
UArchVizManager* AArchVizPlayerController::GetManager()
{
    if (CurrentMode == EArchVizMode::RoadMode)
    {
        return RoadManager;
    }
    if (CurrentMode == EArchVizMode::ConstructionMode)
    {
        return ConstructionManager;
    }
    if (CurrentMode == EArchVizMode::InteriorMode)
    {
        return InteriorManager;
    }
    if (CurrentMode == EArchVizMode::SaveLoadMode)
    {
        return SaveLoadManager;
    }
    if(CurrentMode == EArchVizMode::ExteriorMode)
    {
        return ExteriorManager;
    }
    return nullptr;
}

void AArchVizPlayerController::ChangeMode(EArchVizMode SetMode)
{
    if (GetManager())
    {
        GetManager()->End();
    }
    if (SetMode != CurrentMode)
    {
        CurrentSelectedActor = nullptr;
    }
    CurrentMode = SetMode;
    if (GetManager())
    {
        GetManager()->Start();
    }
}

EArchVizMode AArchVizPlayerController::GetCurrentMode()
{
    return CurrentMode;
}





void AArchVizPlayerController::Load()
{
    SaveLoadManager->LoadSlot(RoadManager->RoadConstructionActors, ConstructionManager->WallActors, ConstructionManager->SlabActors);
}

void AArchVizPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

TArray<FString>  AArchVizPlayerController::FindFiles(FString Path, FString Extension)

{

    TArray<FString> FileList;

    IFileManager& FileManager = IFileManager::Get();

    if (!Path.EndsWith("/"))

    {

        Path += "/";

    }

    FString FullPath = Path + TEXT("*") + Extension;

    FileManager.FindFiles(FileList, *FullPath, true, false);

    return FileList;

}