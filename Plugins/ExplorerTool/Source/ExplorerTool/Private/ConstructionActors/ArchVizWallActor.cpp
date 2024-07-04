#include "ConstructionActors/ArchVizWallActor.h"
#include "ConstructionActors/ArchVizDoorWallActor.h"
#include "Game/ArchVizPlayerController.h"

// Sets default values
AArchVizWallActor::AArchVizWallActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshAsset(TEXT("/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400"));
    if (WallMeshAsset.Succeeded())
    {
        WallMesh = WallMeshAsset.Object;
    }
    StartPoint = FVector(0);
    EndPoint = FVector(0);
    Rotation = FRotator::ZeroRotator;
}

void AArchVizWallActor::BeginPlay()
{
    Super::BeginPlay();
}

void AArchVizWallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (GetIsMoving() && !GetIsConstructed() && GetPlayerController())
    {
        AddEndPoint(GetPlayerController()->GetMouseLocation({ this }).Location);
    }
    else if (GetIsMoving() && GetIsConstructed() && GetPlayerController())
    {
        FVector NewLocation = GetPlayerController()->GetMouseLocation({ this }).Location;
        PlaceWall(NewLocation);
    }
}

void AArchVizWallActor::AddStartPoint(FVector SP)
{
    StartPoint = SnapToGrid(SP, FVector(GridCellSize, GridCellSize, 20.0f));
    SetIsMoving(true);
}

void AArchVizWallActor::AddEndPoint(FVector EP)
{
    for (int32 i = SpawnedWalls.Num() - 1; i >= 0; --i)
    {
        if (SpawnedWalls.IsValidIndex(i))
        {
            UStaticMeshComponent* WallToRemove = SpawnedWalls[i];
            if (WallToRemove)
            {
                WallToRemove->DestroyComponent();
                WallToRemove = nullptr;
            }
        }
    }
    SpawnedWalls.Empty();

    EndPoint = SnapToGrid(EP, FVector(GridCellSize, GridCellSize, 20.0f));

    SpawnWall();
}

void AArchVizWallActor::SpawnWall()
{
    float Distance = 0;
    if (FMath::Abs(StartPoint.X - EndPoint.X) > FMath::Abs(StartPoint.Y - EndPoint.Y))
    {
        Distance = FMath::Abs(StartPoint.X - EndPoint.X);
        EndPoint.Y = StartPoint.Y;
    }
    else
    {
        Distance = FMath::Abs(StartPoint.Y - EndPoint.Y);
        EndPoint.X = StartPoint.X;
    }

    NumberOfWalls = FMath::CeilToInt(Distance / 400.0f);
    //bHasDoorWall.Reserve(NumberOfWalls);
    for (int i = 0; i < NumberOfWalls; i++)
    {
        bHasDoorWall.Add(false);
    }
    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

    FVector RoundedDirection;
    if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Y))
    {
        RoundedDirection = FVector(FMath::RoundToInt(Direction.X), 0.0f, 0.0f).GetSafeNormal();
    }
    else
    {
        RoundedDirection = FVector(0.0f, FMath::RoundToInt(Direction.Y), 0.0f).GetSafeNormal();
    }

    Rotation = RoundedDirection.Rotation();

    FVector MidPoint = (StartPoint + EndPoint) / 2.0f;
    SetActorLocation(MidPoint);

    for (int i = 0; i < NumberOfWalls; ++i)
    {
        FVector SpawnLocation = StartPoint + RoundedDirection * i * 400.0f;

        FVector Offset = FVector::ZeroVector;

        float Yaw = Rotation.Yaw;

        if (FMath::Abs(Yaw) <= 5 || FMath::Abs(Yaw - 360) <= 5)
        {
            Offset.X = WallWidth / 2.0f;
        }
        else if (FMath::Abs(Yaw - 180) <= 5)
        {
            Offset.X = -WallWidth / 2.0f;
        }
        else if (FMath::Abs(Yaw - 90) <= 5)
        {
            Offset.Y = WallWidth / 2.0f;
        }
        else if (FMath::Abs(Yaw + 90) <= 5)
        {
            Offset.Y = -WallWidth / 2.0f;
        }

        SpawnLocation += Offset;
        SpawnMesh(SpawnLocation);
    }
}
void AArchVizWallActor::SpawnDoorActor(FVector SpawnLocation)
{
    FActorSpawnParameters SpawnParams;
    AArchVizDoorWallActor* DoorWall = GetWorld()->SpawnActor<AArchVizDoorWallActor>(SpawnLocation, GetRotation(), SpawnParams);
    if (DoorWall)
    {
        DoorWall->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepWorldTransform);
        if (Material)
        {
            DoorWall->SetMaterial(Material);
        }
        DoorWalls.Add(DoorWall);
    }
}
void AArchVizWallActor::SpawnMesh(FVector SpawnLocation)
{
    UStaticMeshComponent* NewWall = NewObject<UStaticMeshComponent>(this);
    if (NewWall)
    {
        NewWall->SetWorldLocation(SpawnLocation);
        NewWall->SetWorldRotation(Rotation);
        NewWall->SetStaticMesh(WallMesh);
        NewWall->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepWorldTransform);
        NewWall->RegisterComponent();
        NewWall->SetVisibility(true);
        if(Material)
        {
            NewWall->SetMaterial(0,Material);
        }
        SpawnedWalls.Add(NewWall);
    }
}
void AArchVizWallActor::ReplaceWallWithDoor(FVector HitLocation)
{
    float ClosestDistance = FLT_MAX;
    int32 ClosestIndex = -1;

    for (int32 i = 0; i < SpawnedWalls.Num(); ++i)
    {
        float Distance = FVector::Dist(SpawnedWalls[i]->GetComponentLocation(), HitLocation);
        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestIndex = i;
        }
    }

    FVector WallLocation = FVector::ZeroVector;
    if (ClosestIndex != -1)
    {
        //bHasDoorWall.Add(ClosestIndex);

        UStaticMeshComponent* WallToRemove = SpawnedWalls[ClosestIndex];
        if (WallToRemove)
        {
            WallLocation = WallToRemove->GetComponentLocation();
            WallToRemove->DestroyComponent();
            WallToRemove = nullptr;
            SpawnedWalls.RemoveAt(ClosestIndex);
        }
        int32 wallI = GetWallIndexFromLocation(WallLocation);
        
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Wall Index: %d"), wallI));

        bHasDoorWall[wallI] = true;
        
        SpawnDoorActor(WallLocation);
    }
}
void AArchVizWallActor::ReplaceWallWithDoor(int32 index)
{
    FVector Location = GetLocationFromIndex(index);

    ReplaceWallWithDoor(Location);
    
}
void AArchVizWallActor::PlaceWall(FVector NewLocation)
{
    FVector SnappedLocation = SnapToGrid(NewLocation, FVector(GridCellSize, GridCellSize, 20.0f));

    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
    float Distance = FVector::Dist(StartPoint, EndPoint);

    StartPoint = SnappedLocation;
    EndPoint = StartPoint + Direction * Distance;

    FVector MidPoint = (StartPoint + EndPoint) / 2.0f;
    SetActorLocation(MidPoint);
}

void AArchVizWallActor::ReplaceDoorWithWall(AArchVizDoorWallActor* DoorWallActor)
{
    if (DoorWallActor)
    {
        FVector DoorWallLocation = DoorWallActor->GetActorLocation();
        DoorWallActor->Destroy();

        int32 IndexToRemove = DoorWalls.IndexOfByKey(DoorWallActor);
        if (IndexToRemove != INDEX_NONE)
        {
            DoorWalls.RemoveAt(IndexToRemove);
        }
        int32 doorwallI = GetWallIndexFromLocation(DoorWallLocation);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Wall Index: %d"), doorwallI));
        bHasDoorWall[doorwallI] = false;
        SpawnMesh(DoorWallLocation);
    }
}

void AArchVizWallActor::SetMaterial(UMaterialInterface* NewMaterial)
{
    if (NewMaterial)
    {
        Material = NewMaterial;
        for (UStaticMeshComponent* Wall : SpawnedWalls)
        {
            if (Wall)
            {
                Wall->SetMaterial(0, Material);
            }
        }
        for (AArchVizDoorWallActor* DoorWall : DoorWalls)
        {
            if (DoorWall)
            {
                DoorWall->SetMaterial(Material);
            }
        }
    }
}

void AArchVizWallActor::RotateActor(FRotator InRotation)
{
    Super::RotateActor(InRotation);
    FRotator currentRotation = GetActorRotation();

    double newYaw = fmod(currentRotation.Yaw + 90.0, 360.0);
    if (newYaw < 0)
    {
        newYaw += 360.0;
    }

    FRotator newRotation = FRotator(currentRotation.Pitch, newYaw, currentRotation.Roll);

    SetActorRotation(newRotation);
}

void AArchVizWallActor::HighlightSelectedActor()
{
    TSet<UActorComponent*> ActorComponents = GetComponents();

    for (auto& ActorComponent : ActorComponents)
    {
        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent))
        {
            Component->SetRenderCustomDepth(true);
            Component->CustomDepthStencilValue = 2;
        }
    }

    for (AArchVizDoorWallActor* DoorWall : DoorWalls)
    {
        if (DoorWall)
        {
            DoorWall->WallComponent->SetRenderCustomDepth(true);
            DoorWall->WallComponent->CustomDepthStencilValue = 2;
            DoorWall->DoorActor->UnhighlightDeselectedActor();
        }
    }
}

void AArchVizWallActor::UnhighlightDeselectedActor()
{
    TSet<UActorComponent*> ActorComponents = GetComponents();

    for (auto& ActorComponent : ActorComponents)
    {
        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent))
        {
            Component->SetRenderCustomDepth(false);
        }
    }

    for (AArchVizDoorWallActor* DoorWall : DoorWalls)
    {
        if (DoorWall)
        {
            DoorWall->WallComponent->SetRenderCustomDepth(false);
        }
    }
}

int32 AArchVizWallActor::GetWallIndexFromLocation(FVector Location) const
{
    FVector WallDirection = (EndPoint - StartPoint).GetSafeNormal();
    float DistanceAlongWall = FVector::DotProduct(Location - StartPoint, WallDirection);

    int32 WallIndex = FMath::FloorToInt(DistanceAlongWall / WallLenght);
    WallIndex = FMath::Clamp(WallIndex, 0, bHasDoorWall.Num() - 1);

    return WallIndex;
}
FVector AArchVizWallActor::GetLocationFromIndex(int32 Index) const
{
    // Ensure the index is within bounds
    if (Index < 0 || Index >= NumberOfWalls)
    {
        return FVector::ZeroVector; // Return zero vector if index is out of bounds
    }

    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();

    FVector RoundedDirection;
    if (FMath::Abs(Direction.X) >= FMath::Abs(Direction.Y))
    {
        RoundedDirection = FVector(FMath::RoundToInt(Direction.X), 0.0f, 0.0f).GetSafeNormal();
    }
    else
    {
        RoundedDirection = FVector(0.0f, FMath::RoundToInt(Direction.Y), 0.0f).GetSafeNormal();
    }

    FVector SpawnLocation = StartPoint + RoundedDirection * Index * 400.0f;

    FVector Offset = FVector::ZeroVector;

    float Yaw = Rotation.Yaw;

    if (FMath::Abs(Yaw) <= 5 || FMath::Abs(Yaw - 360) <= 5)
    {
        Offset.X = WallWidth / 2.0f;
    }
    else if (FMath::Abs(Yaw - 180) <= 5)
    {
        Offset.X = -WallWidth / 2.0f;
    }
    else if (FMath::Abs(Yaw - 90) <= 5)
    {
        Offset.Y = WallWidth / 2.0f;
    }
    else if (FMath::Abs(Yaw + 90) <= 5)
    {
        Offset.Y = -WallWidth / 2.0f;
    }

    SpawnLocation += Offset;
    return SpawnLocation;
}
