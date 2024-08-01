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

        FHitResult HitResult;
        PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

        if (AArchVizWallActor* Actor = Cast<AArchVizWallActor>(HitResult.GetActor()))
        {
            if (ComponentType == EInteriorItemType::WallPlaceable)
            {
                // Calculate wall normal
                FVector WallNormal = HitResult.Normal;

                // Determine if the wall normal is facing inward or outward
                /*if (FVector::DotProduct(WallNormal, Actor->GetActorForwardVector()) > 0)
                {
                    WallNormal = -WallNormal;
                }*/
                //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("HitResult.Normal: %s"), *WallNormal.ToString()));

                // Calculate object rotation based on wall normal
                FRotator ObjectRotation = WallNormal.Rotation();
                ObjectRotation.Pitch = 0;  // Ensure the object is not tilted

                //// Adjust rotation by 90 degrees to align the front of the actor with the wall
                ObjectRotation.Yaw -= 90.0f;
                if (ObjectRotation.Yaw < 0)
                {
                    ObjectRotation.Yaw += 360.0;
                }
                SetActorRotation(ObjectRotation);

                
                FVector WallLocation = HitResult.Location;

                SetActorLocation(WallLocation);
            }
            else
            {
                SetActorLocation(HitResult.Location);
            }
        }
        else
        {
            SetActorLocation(HitResult.Location);
        }

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
