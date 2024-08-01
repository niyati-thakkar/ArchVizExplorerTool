// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadActors/ArchVizRoadActor.h"

#include "Kismet/GameplayStatics.h"
#include "SaveLoad/ArchVizSaveTool.h"


// Sets default values
// Sets default values
AArchVizRoadActor::AArchVizRoadActor()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create and set the root component
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    // Load the static mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor"));
    if (StaticMeshAsset.Succeeded())
    {
        RoadMesh = StaticMeshAsset.Object;
    }

    // Load the material
    static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset(TEXT("/ExplorerTool/RoadConstruction/TwoYellowLines.TwoYellowLines"));
    if (MaterialAsset.Succeeded())
    {
        RoadMaterial = MaterialAsset.Object;
    }

    // Initialize the spline component
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(SceneComponent);  // Correct use of SetupAttachment during initialization
    SplineComponent->ClearSplinePoints();

    // Set the default road type
    RoadType = ERoadType::StraightRoad;
}


// Called when the game starts or when spawned
void AArchVizRoadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArchVizRoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArchVizRoadActor::AddRoadPoint(FVector SplineLocation)
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

void AArchVizRoadActor::UpdateRoadMeshes()
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

        FVector2D Scale = FVector2D(RoadWidth / 100, 1.0f);

        USplineMeshComponent* MeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        MeshComponent->SetMobility(EComponentMobility::Movable);
        MeshComponent->SetStaticMesh(RoadMesh);
        MeshComponent->SetMaterial(0, RoadMaterial);
        MeshComponent->RegisterComponent();
        MeshComponent->SetStartScale(Scale);
        MeshComponent->SetEndScale(Scale);
        MeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true); // 'true' ensures tangents are mirrored correctly
        MeshComponent->SetForwardAxis(ESplineMeshAxis::X);
        MeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

        // Set collision profile to use simple collision as complex
        MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);


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

        FVector2D Scale = FVector2D(RoadWidth / 100, 1.0f);
        USplineMeshComponent* MeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        RoadMesh->SetMaterial(0, RoadMaterial);
        MeshComponent->SetStaticMesh(RoadMesh);
        //MeshComponent->SetMaterial(0, RoadMaterial);
        MeshComponent->RegisterComponent();
        MeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true); // 'true' ensures tangents are mirrored correctly
        MeshComponent->SetForwardAxis(ESplineMeshAxis::X);
        MeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);
        MeshComponent->SetStartScale(Scale);
        MeshComponent->SetEndScale(Scale);
        // Optionally set the up vector to prevent twisting
        MeshComponent->SetSplineUpDir(FVector::UpVector, true);
        MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

        // Set collision profile to use simple collision as complex
        MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

        SplineMeshes.Add(MeshComponent);
    }
}
//void AArchVizRoadActor::SaveRoadState(const FString& SlotName)
//{
//    UArchVizSaveTool* SaveGameInstance = Cast<UArchVizSaveTool>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveTool::StaticClass()));
//
//    FConstructedRoad ConstructedRoad;
//    for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
//    {
//        ConstructedRoad.SplinePoints.Add(SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local));
//    }
//
//    ConstructedRoad.RoadType = RoadType;
//
//    if (RoadMaterial)
//    {
//        ConstructedRoad.RoadMaterialPath = RoadMaterial->GetPathName();
//    }
//
//    if (RoadMesh)
//    {
//        ConstructedRoad.MeshComponentPath = RoadMesh->GetPathName();
//    }
//
//    SaveGameInstance->GameSlots.FindOrAdd(SlotName).RoadElements.Add(ConstructedRoad);
//
//    UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("RoadSaveSlot"), 0);
//}
//
//void AArchVizRoadActor::LoadRoadState(const FString& SlotName)
//{
//    UArchVizSaveTool* LoadGameInstance = Cast<UArchVizSaveTool>(UGameplayStatics::LoadGameFromSlot(TEXT("RoadSaveSlot"), 0));
//    if (LoadGameInstance)
//    {
//        const FSaveSlotElement* SlotElement = LoadGameInstance->GameSlots.Find(SlotName);
//        if (SlotElement)
//        {
//            for (const FConstructedRoad& ConstructedRoad : SlotElement->RoadElements)
//            {
//                SplineComponent->ClearSplinePoints();
//                for (const FVector& Point : ConstructedRoad.SplinePoints)
//                {
//                    SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::Local);
//                }
//
//                RoadType = ConstructedRoad.RoadType;
//
//                if (!ConstructedRoad.RoadMaterialPath.IsEmpty())
//                {
//                    RoadMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *ConstructedRoad.RoadMaterialPath));
//                }
//
//                if (!ConstructedRoad.MeshComponentPath.IsEmpty())
//                {
//                    RoadMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *ConstructedRoad.MeshComponentPath));
//                }
//
//                UpdateRoadMeshes();
//            }
//        }
//    }
//}

void AArchVizRoadActor::RemoveLastPoint()
{
    if (SplineComponent->GetNumberOfSplinePoints() > 0)
    {
        SplineComponent->RemoveSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1);
    }

}