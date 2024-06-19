// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadConstructionActor.h"

#include "ArchVizSaveTool.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARoadConstructionActor::ARoadConstructionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    // Load the static mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor"));
    if (StaticMeshAsset.Succeeded())
    {
        RoadMesh = StaticMeshAsset.Object;

        // Load the material
        static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/ExplorerTool/RoadMaterial/MI_Fine_Road_vgdlejpew_2K.MI_Fine_Road_vgdlejpew_2K"));
        if (MaterialAsset.Succeeded())
        {
            RoadMaterial = MaterialAsset.Object;
        }
    }

    // Initialize the first spline component
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(SceneComponent);
    SplineComponent->ClearSplinePoints();
    RoadType = ERoadType::StraightRoad;
}

// Called when the game starts or when spawned
void ARoadConstructionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoadConstructionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoadConstructionActor::AddRoadPoint(FVector SplineLocation)
{
    SplineLocation.Z = 20;
    SplineComponent->AddSplinePoint(SplineLocation, ESplineCoordinateSpace::Local);

    int32 SplinePoints = SplineComponent->GetNumberOfSplinePoints();
    if (SplinePoints < 2)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Add one more point"));
    }else
    {
        UpdateRoadMeshes();
    }
        
}

void ARoadConstructionActor::UpdateRoadMeshes()
{
    // Remove existing meshes
    for (auto* Mesh : SplineMeshes)
    {
        if (Mesh)
        {
            Mesh->DestroyComponent();
        }
        Mesh = nullptr;
    }
    SplineMeshes.Empty();

    if (!RoadMesh || !RoadMaterial)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RoadMesh or RoadMaterial is not set."));
       
    }

    float SplineLength = SplineComponent->GetSplineLength();

    // Calculate segment length using the bounding box of the static mesh
    FBoxSphereBounds MeshBounds = RoadMesh->GetBounds();
    float SegmentLength = MeshBounds.BoxExtent.X * 2.0f; // Assuming X axis is the length direction

    int32 NumSegments = FMath::Floor(SplineLength / SegmentLength);

    // Set spline points to Linear
    if (RoadType == ERoadType::StraightRoad)
    {
        for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
        {
            SplineComponent->SetSplinePointType(i, ESplinePointType::Linear, true);
        }
    }else
    {
        for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
        {
            SplineComponent->SetSplinePointType(i, ESplinePointType::Curve, true);
        }
    }

    // Place the road meshes
    for (int32 Index = 0; Index < NumSegments; ++Index)
    {
        float StartDistance = SegmentLength * Index;
        float EndDistance = SegmentLength * (Index + 1);

        FVector StartPos = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
        FVector EndPos = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
        FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
        FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

        StartTangent = StartTangent.GetClampedToMaxSize(SegmentLength);
        EndTangent = EndTangent.GetClampedToMaxSize(SegmentLength);

        USplineMeshComponent* MeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        MeshComponent->SetStaticMesh(RoadMesh);
        MeshComponent->SetMaterial(0, RoadMaterial);
        MeshComponent->RegisterComponent();
        MeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true); // 'true' ensures tangents are mirrored correctly
        MeshComponent->SetForwardAxis(ESplineMeshAxis::X);
        MeshComponent->SetupAttachment(SplineComponent);

        SplineMeshes.Add(MeshComponent);
    }

    if (NumSegments * SegmentLength < SplineLength && RoadType == ERoadType::CurvedRoad)
    {
        float StartDistance = NumSegments * SegmentLength;
        float EndDistance = SplineLength;

        FVector StartPos = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
        FVector EndPos = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

        float LastSegmentLength = EndDistance - StartDistance;
        FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
        FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

        // Scale tangents proportionally to the actual segment length
        StartTangent = StartTangent.GetClampedToMaxSize(LastSegmentLength);
        EndTangent = EndTangent.GetClampedToMaxSize(LastSegmentLength);

        USplineMeshComponent* MeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        MeshComponent->SetStaticMesh(RoadMesh);
        MeshComponent->SetMaterial(0, RoadMaterial);
        MeshComponent->RegisterComponent();
        MeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true); // 'true' ensures tangents are mirrored correctly
        MeshComponent->SetForwardAxis(ESplineMeshAxis::X);
        MeshComponent->SetupAttachment(SplineComponent);

        // Optionally set the up vector to prevent twisting
        MeshComponent->SetSplineUpDir(FVector::UpVector, true);

        SplineMeshes.Add(MeshComponent);
    }
}
void ARoadConstructionActor::SaveRoadState(const FString& SlotName)
{
    UArchVizSaveTool* SaveGameInstance = Cast<UArchVizSaveTool>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveTool::StaticClass()));

    FConstructedRoad ConstructedRoad;
    for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
    {
        ConstructedRoad.SplinePoints.Add(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local));
    }

    ConstructedRoad.RoadType = RoadType;

    if (RoadMaterial)
    {
        ConstructedRoad.RoadMaterialPath = RoadMaterial->GetPathName();
    }

    if (RoadMesh)
    {
        ConstructedRoad.MeshComponentPath = RoadMesh->GetPathName();
    }

    SaveGameInstance->GameSlots.FindOrAdd(SlotName).RoadElements.Add(ConstructedRoad);

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("RoadSaveSlot"), 0);
}

void ARoadConstructionActor::LoadRoadState(const FString& SlotName)
{
    UArchVizSaveTool* LoadGameInstance = Cast<UArchVizSaveTool>(UGameplayStatics::LoadGameFromSlot(TEXT("RoadSaveSlot"), 0));
    if (LoadGameInstance)
    {
        const FSaveSlotElement* SlotElement = LoadGameInstance->GameSlots.Find(SlotName);
        if (SlotElement)
        {
            for (const FConstructedRoad& ConstructedRoad : SlotElement->RoadElements)
            {
                SplineComponent->ClearSplinePoints();
                for (const FVector& Point : ConstructedRoad.SplinePoints)
                {
                    SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::Local);
                }

                RoadType = ConstructedRoad.RoadType;

                if (!ConstructedRoad.RoadMaterialPath.IsEmpty())
                {
                    RoadMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *ConstructedRoad.RoadMaterialPath));
                }

                if (!ConstructedRoad.MeshComponentPath.IsEmpty())
                {
                    RoadMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *ConstructedRoad.MeshComponentPath));
                }

                UpdateRoadMeshes();
            }
        }
    }
}