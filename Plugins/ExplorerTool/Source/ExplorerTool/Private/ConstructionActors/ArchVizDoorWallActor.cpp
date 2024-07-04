
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

    // Load the wall mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorWallMeshAsset(TEXT("/Game/StarterContent/Architecture/Wall_Door_400x400.Wall_Door_400x400"));
    if (DoorWallMeshAsset.Succeeded())
    {
        DoorWallMesh = DoorWallMeshAsset.Object;
        WallComponent->SetStaticMesh(DoorWallMesh);
    }

}

// Called when the game starts or when spawned
void AArchVizDoorWallActor::BeginPlay()
{
    Super::BeginPlay();

    // Spawn the door actor and attach it to the wall
    if (GetWorld())
    {
        DoorActor = GetWorld()->SpawnActor<AArchVizDoorActor>();
        if (DoorActor)
        {
            DoorActor->AttachToComponent(WallComponent, FAttachmentTransformRules::KeepRelativeTransform);
            DoorActor->SetActorRelativeLocation(FVector(200.0f, 0.0f, 0.0f)); // Adjust as necessary
        }
    }
    
}

// Called every frame
void AArchVizDoorWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void AArchVizDoorWallActor::SetMaterial(UMaterialInterface* NewMaterial)
{
    WallComponent->SetMaterial(0, NewMaterial);
}


