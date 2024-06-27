
#include "ConstructionActors/ArchVizDoorWallActor.h"

#include "Components/StaticMeshComponent.h"


// Sets default values
AArchVizDoorWallActor::AArchVizDoorWallActor()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and set up the wall component
    WallComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallComponent"));
    RootComponent = WallComponent;

    // Create and set up the door frame component
    DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameComponent"));
    DoorFrameComponent->SetupAttachment(RootComponent);
    DoorFrameComponent->SetRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
    DoorFrameComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    DoorFrameComponent->SetRelativeScale3D(FVector(1, 1.1f, 1));

    // Create and set up the door component
    DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorComponent"));
    DoorComponent->SetupAttachment(DoorFrameComponent);
    DoorComponent->SetRelativeLocation(FVector(0.0f, 45.0f, 0.0f));
    
    DoorComponent->SetRelativeScale3D(FVector(1, 1.f, 1));
    // Load the wall mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorWallMeshAsset(TEXT("/Game/StarterContent/Architecture/Wall_Door_400x400.Wall_Door_400x400"));
    if (DoorWallMeshAsset.Succeeded())
    {
        DoorWallMesh = DoorWallMeshAsset.Object;
        WallComponent->SetStaticMesh(DoorWallMesh);
    }

    // Load the door frame mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFrameMeshAsset(TEXT("/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame"));
    if (DoorFrameMeshAsset.Succeeded())
    {
        DoorFrameMesh = DoorFrameMeshAsset.Object;
        DoorFrameComponent->SetStaticMesh(DoorFrameMesh);
    }

    // Load the door mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorMeshAsset(TEXT("/Game/StarterContent/Props/SM_Door.SM_Door"));
    if (DoorMeshAsset.Succeeded())
    {
        DoorMesh = DoorMeshAsset.Object;
        DoorComponent->SetStaticMesh(DoorMesh);
    }
}

// Called when the game starts or when spawned
void AArchVizDoorWallActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AArchVizDoorWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void AArchVizDoorWallActor::OpenDoor()
{
    DoorComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}
void AArchVizDoorWallActor::CloseDoor()
{
    DoorComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}