// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructionActors/ArchVizSlabActor.h"
#include "ConstructionActors/ArchVizWallActor.h"
#include "Game/ArchVizPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

void AArchVizSlabActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
    SpawnSlabs();
}

AArchVizSlabActor::AArchVizSlabActor()
{
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SlabMeshAsset(TEXT("/ExplorerTool/SlabMesh.SlabMesh"));
    if (SlabMeshAsset.Succeeded())
    {
        SlabMesh = SlabMeshAsset.Object;
    }

    //StartPoint = FVector(100);
    //EndPoint = FVector(100);

    // Create and register the procedural mesh component
    ProceduralSlabMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralSlabMesh"));
    ProceduralSlabMesh->SetupAttachment(RootComponent);
    SlabSizeX = 10.0f;
    SlabSizeY = 10.0f;
    SlabSizeZ = 10.0f;
    
}

// Called when the game starts or when spawned
void AArchVizSlabActor::BeginPlay()
{
    Super::BeginPlay();
    ProceduralSlabMesh->RegisterComponent();
}

// Called every frame
void AArchVizSlabActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetIsMoving() && !GetIsConstructed() && GetPlayerController())
    {
        AddEndPoint(GetPlayerController()->GetMouseLocation({ this }).Location);
    }
    else if (GetIsMoving() && GetIsConstructed() && GetPlayerController())
    {
        FVector NewLocation = GetPlayerController()->GetMouseLocation({ this }).Location;
        PlaceSlab(NewLocation);
    }
}

void AArchVizSlabActor::AddStartPoint(FVector SP)
{
    StartPoint = SnapToGrid(SP, FVector(20.0f));
    
    SetIsMoving(true);
 
}

void AArchVizSlabActor::AddEndPoint(FVector EP)
{
    EndPoint = SnapToGrid(EP, FVector(20.0f));
    SpawnSlabs();
}

void AArchVizSlabActor::SpawnSlabs()
{
    // Calculate dimensions based on start and end points
    SlabSizeX = (EndPoint.X - StartPoint.X);
    SlabSizeY = (EndPoint.Y - StartPoint.Y);
	/*float SlabSizeX = 100;
    float SlabSizeY = 100;*/

    FVector SpawnLocation = StartPoint; // Center point for spawning
    SpawnLocation.Z = StartPoint.Z;
    // Ensure ProceduralSlabMesh is valid before using it
    if (ProceduralSlabMesh)
    {
        // Clear any previous mesh data
        ProceduralSlabMesh->ClearAllMeshSections();

        // Generate the floor mesh
        GenerateSlab();

        // Set the location of the mesh
        ProceduralSlabMesh->SetWorldLocation(SpawnLocation);

    }
}
void AArchVizSlabActor::BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent)
{
    const int32 Index1 = VertexOffset++;
    const int32 Index2 = VertexOffset++;
    const int32 Index3 = VertexOffset++;
    const int32 Index4 = VertexOffset++;
    InVertices[Index1] = BottomLeft;
    InVertices[Index2] = BottomRight;
    InVertices[Index3] = TopRight;
    InVertices[Index4] = TopLeft;
    InTexCoords[Index1] = FVector2D(0.0f, 1.0f);
    InTexCoords[Index2] = FVector2D(1.0f, 1.0f);
    InTexCoords[Index3] = FVector2D(1.0f, 0.0f);
    InTexCoords[Index4] = FVector2D(0.0f, 0.0f);
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index2;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index4;
    // On a cube side, all the vertex normals face the same way
    InNormals[Index1] = InNormals[Index2] = InNormals[Index3] = InNormals[Index4] = Normal;
    InTangents[Index1] = InTangents[Index2] = InTangents[Index3] = InTangents[Index4] = Tangent;
}

void AArchVizSlabActor::SetMaterial(UMaterialInterface* Material, int Segment)
{
    if(Segment == 0)
    {
        RoofMaterial = Material;
    }else
    {
        FloorMaterial = Material;
    }
    if (IsValid(Material))
    {
        
    	UMaterialInstanceDynamic* DynTopMaterial = UMaterialInstanceDynamic::Create(Material, this);
        

        if (DynTopMaterial)
        {
            DynTopMaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(SlabSizeX / 400.0f, SlabSizeY / 400.0f, 0, 0));
            ProceduralSlabMesh->SetMaterial(Segment, DynTopMaterial);

        }
    }
    
}

void AArchVizSlabActor::RotateActor(FRotator InRotation)
{

    Super::RotateActor(InRotation);
    FRotator currentRotation = ProceduralSlabMesh->GetRelativeRotation();

    double newYaw = fmod(currentRotation.Yaw + 90.0, 360.0);
    if (newYaw < 0)
    {
        newYaw += 360.0;
    }

    // Create a new rotation with the updated Yaw
    FRotator newRotation = FRotator(currentRotation.Pitch, newYaw, currentRotation.Roll);

    // Set the actor's rotation to the new rotation
    ProceduralSlabMesh->SetRelativeRotation(newRotation);
}

void AArchVizSlabActor::GenerateCube(FVector StartLocation, int Segment)
{


    float x = SlabSizeX;
    float y = SlabSizeY;
    float z = SlabSizeZ;

    TArray<FVector> Vertices;
    TArray<FVector> Normals;
    TArray<FProcMeshTangent> Tangents;
    TArray<FVector2D> UVs;
    TArray<FColor> Colors;
    TArray<int32> Triangles;

    Vertices.Add(StartLocation + FVector(0, y, 0));    // 0
	Vertices.Add(StartLocation + FVector(x, y, 0));    // 1
	Vertices.Add(StartLocation + FVector(x, y, z));    // 2
	Vertices.Add(StartLocation + FVector(0, y, z));    // 3

	Vertices.Add(StartLocation + FVector(x, y, 0));    // 4
	Vertices.Add(StartLocation + FVector(x, 0, 0));    // 5
	Vertices.Add(StartLocation + FVector(x, 0, z));    // 6
	Vertices.Add(StartLocation + FVector(x, y, z));    // 7

	Vertices.Add(StartLocation + FVector(0, y, z));    // 8
	Vertices.Add(StartLocation + FVector(x, y, z));    // 9
	Vertices.Add(StartLocation + FVector(x, 0, z));    // 10
	Vertices.Add(StartLocation + FVector(0, 0, z));    // 11

	Vertices.Add(StartLocation + FVector(0, 0, 0));    // 12
	Vertices.Add(StartLocation + FVector(0, y, 0));    // 13
	Vertices.Add(StartLocation + FVector(0, y, z));    // 14
	Vertices.Add(StartLocation + FVector(0, 0, z));    // 15

	Vertices.Add(StartLocation + FVector(x, 0, 0));    // 16
	Vertices.Add(StartLocation + FVector(0, 0, 0));    // 17
	Vertices.Add(StartLocation + FVector(0, 0, z));    // 18
	Vertices.Add(StartLocation + FVector(x, 0, z));    // 19

	Vertices.Add(StartLocation + FVector(0, 0, 0));    // 20
	Vertices.Add(StartLocation + FVector(x, 0, 0));    // 21
	Vertices.Add(StartLocation + FVector(x, y, 0));    // 22
	Vertices.Add(StartLocation + FVector(0, y, 0));    // 23

    /*Vertices.Add(FVector(-x, y, -z)); Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(x, y, z)); Vertices.Add(FVector(-x, y, z));
    Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(x, -y, z)); Vertices.Add(FVector(x, y, z));
    Vertices.Add(FVector(-x, y, z)); Vertices.Add(FVector(x, y, z)); Vertices.Add(FVector(x, -y, z)); Vertices.Add(FVector(-x, -y, z));
    Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(-x, y, -z)); Vertices.Add(FVector(-x, y, z)); Vertices.Add(FVector(-x, -y, z));
    Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(-x, -y, z)); Vertices.Add(FVector(x, -y, z));
    Vertices.Add(FVector(-x, -y, -z)); Vertices.Add(FVector(x, -y, -z)); Vertices.Add(FVector(x, y, -z)); Vertices.Add(FVector(-x, y, -z));*/

   /* Triangles.Append
    ({
        0,1,2, 0,2,3,
        4,5,6, 7,4,6,
        8,9,10, 11,8,10,
        12,13,14, 15,12,14,
        16,17,18, 19,16,18,
        20,21,22, 23,20,22
        }


    );

    for (int i = 0; i < Triangles.Num(); i += 3)
    {
        FVector temp = (Vertices[Triangles[i + 2]] - Vertices[Triangles[i]]) ^ (Vertices[Triangles[i + 1]] - Vertices[Triangles[i + 2]]);
        Normals.Add(temp);
        Normals.Add(temp);
    }*/

    //Normals = {
    //FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), // Top face
    //FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), // Right face
    //FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), // Front face
    //FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), // Left face
    //FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), // Bottom face
    //FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0)  // Back face
    //};

    // Define the UVs for each vertex
    //UVs = {
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // Top face
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0),
    //    FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // Back face
    //};

    // Define the tangents for each face
    Tangents = {
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Top face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), // Right face
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Front face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), // Left face
        FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), FProcMeshTangent(1, 0, 0), // Bottom face
        FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0), FProcMeshTangent(0, 1, 0)  // Back face
    };
    

    for (int32 i = 0; i < 24; i += 4) {
        AddTriangle(Triangles, i, i + 1, i + 2);
        AddTriangle(Triangles, i + 2, i + 3, i);
    }

    //Normals
    

    for (int32 i = 0; i < 24; i += 4) {
        FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 1], Vertices[i]));

        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
        Normals.Add(CurrentNormal);
    }

    //UVs
    

    for (int32 i = 0; i < 24; i += 4) {
        UVs.Add(FVector2D{ 0.0,0.0 });
        UVs.Add(FVector2D{ 0.0,1.0 });
        UVs.Add(FVector2D{ 1.0,1.0 });
        UVs.Add(FVector2D{ 1.0,0.0 });
    }
    // Optionally, add vertex colors
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        Colors.Add(FColor::White);
    }

    if (Vertices.Num() == Tangents.Num() && UVs.Num() == Colors.Num() && Normals.Num() == Vertices.Num())
    {
        ProceduralSlabMesh->CreateMeshSection(Segment, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
    }


}
void AArchVizSlabActor::AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3) {
    Triangles.Add(Index1);
    Triangles.Add(Index2);
    Triangles.Add(Index3);
}

void AArchVizSlabActor::GenerateSlab()
{
    FVector StartPoint1 = FVector(0, 0, 0);
    FVector StartPoint2 = FVector(0, 0, SlabSizeZ); // Slightly offset to prevent Z-fighting

    GenerateCube(StartPoint1, 0);
    GenerateCube(StartPoint2, 1);
}
void AArchVizSlabActor::PlaceSlab(FVector NewLocation)
{
    // Snap the new location to the grid
    FVector SnappedLocation = SnapToGrid(NewLocation, FVector(20.0f));

    // Calculate the direction and distance between the original start and end points
    FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
    float Distance = FVector::Dist(StartPoint, EndPoint);

    // Set the new start point to the snapped location
    StartPoint = SnappedLocation;

    // Calculate the new end point based on the direction and distance
    EndPoint = StartPoint + Direction * Distance;

    FVector SpawnLocation = (StartPoint + EndPoint) * 0.5f; // Center point for spawning
    SpawnLocation.Z = StartPoint.Z;
    // Spawn the slabs at the new location
    ProceduralSlabMesh->SetWorldLocation(StartPoint);
}