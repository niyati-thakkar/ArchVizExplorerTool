#include "Interior/ArchVizInteriorActor.h"
#include "Game/ArchVizPlayerController.h"
#include "Components/StaticMeshComponent.h"

AArchVizInteriorActor::AArchVizInteriorActor()
{
    PrimaryActorTick.bCanEverTick = true;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void AArchVizInteriorActor::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());
}

void AArchVizInteriorActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (GetIsMoving() && PlayerController)
    {
        // Temporarily disable collision
        SetActorEnableCollision(false);
        FHitResult HitResult = PlayerController->GetMouseLocation({ this });
        
        
        if(AArchVizConstructionActor* Actor = Cast<AArchVizConstructionActor>(HitResult.GetActor()))
        {
	        
            if (ComponentType == EInteriorItemType::WallPlaceable && Actor->IsA(AArchVizWallActor::StaticClass()))
            {
                SetActorRotation(Cast<AArchVizWallActor>(Actor)->GetRotation());
            }

        }
        
        SetActorRelativeLocation(HitResult.Location);
        AdjustPositionForPlacement();

        // Re-enable collision
        SetActorEnableCollision(true);
    }
}
EInteriorItemType AArchVizInteriorActor::GetComponentType()
{
    return ComponentType;
}

bool AArchVizInteriorActor::IsRotateable()
{
	return true;
}

void AArchVizInteriorActor::SetIsMoving(bool isMoving)
{
    bIsMoving = isMoving;
}

void AArchVizInteriorActor::SetStaticMesh(UStaticMesh* StaticMesh, EInteriorItemType ItemType)
{
    Mesh = StaticMesh;
    MeshComponent->SetStaticMesh(Mesh);
    ComponentType = ItemType;
}

void AArchVizInteriorActor::AdjustPositionForPlacement()
{
    FVector Origin;
    FVector BoxExtent;
    GetActorBounds(false, Origin, BoxExtent);

    FVector Adjustment = FVector::ZeroVector;

  
    if (ComponentType == EInteriorItemType::FloorPlaceable) {
        Adjustment.Z = BoxExtent.Z;
    }

    SetActorLocation(GetActorLocation() + Adjustment);
}

void AArchVizInteriorActor::RotateActor(FRotator ApplyRotation)
{
    FRotator currentRotation = GetActorRotation();

    double newYaw = fmod(currentRotation.Yaw + 90.0, 360.0);
    if (newYaw < 0)
    {
        newYaw += 360.0;
    }

    // Create a new rotation with the updated Yaw
    FRotator newRotation = FRotator(currentRotation.Pitch, newYaw, currentRotation.Roll);

    // Set the actor's rotation to the new rotation
    SetActorRotation(newRotation);
}
