// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"

USTRUCT(BlueprintType)
struct FFenceProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SideLength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BottomHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Spacing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HorizontalRadius;

};


USTRUCT(BlueprintType)
struct FFenceClassPair
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Data")
    UStaticMesh* MeshClass;
};
UENUM(BlueprintType)
enum class EFenceType : uint8
{
    RoundedOverTopCapital UMETA(DisplayName = "Rounded Over Top Capital"),
    ACornCapital UMETA(DisplayName = "ACorn Capital"),
    RoundTurnedCapital UMETA(DisplayName = "Round Turned Capital"),
    WindsorTurnedCapital UMETA(DisplayName = "Windsor Turned Capital"),
    GothicStarCapital UMETA(DisplayName = "Gothic Star Capital"),
    PyramidTop UMETA(DisplayName = "Pyramid Top"),
    Random UMETA(DisplayName = "Random")
};

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
    Oak_Wood UMETA(DisplayName = "Oak Wood"),
    Pine_Wood UMETA(DisplayName = "Pine Wood"),
    Walnut_Wood UMETA(DisplayName = "Walnut Wood"),
    Walnut_Floor_Wood UMETA(DisplayName = "Walnut Floor Wood"),
    Random UMETA(DisplayName = "Random")
};

UCLASS()
class EXPLORERTOOL_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalRailActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	static void SelectForRandom(EFenceType& FType, EMaterialType& EType);

	virtual void OnConstruction(const FTransform& Transform) override;

public:
    UFUNCTION(BlueprintCallable, Category = Defaults)
    void InitialPillarGeneration();
        
    int32 Segment = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UProceduralMeshComponent* ProcMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rail Settings")
    float TopMeshLerpValue;

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFenceType FenceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMaterialType MaterialType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FFenceProperties FenceProperty;

    UFUNCTION(BlueprintCallable, Category = Defaults)
    void RoundTurnedCapital();

	UFUNCTION(BlueprintCallable, Category = Defaults)
    void WindsorTurnedCapital();

    UFUNCTION(BlueprintCallable, Category = Defaults)
	void PyramidTop();

    UFUNCTION(BlueprintCallable, Category = Defaults)
	void ACornCapital();

    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GothicStarCapital();

    UFUNCTION(BlueprintCallable, Category = Defaults)
	void RoundedOverTopCapital();



	UFUNCTION(BlueprintCallable, Category = Defaults)
    void GenerateCube(FVector Dimensions);

    UFUNCTION(BlueprintCallable, Category = Defaults)
    void GenerateSphere(float Radius, int32 Segments, int32 Rings);

    UFUNCTION(BlueprintCallable, Category = Defaults)
    void GenerateCone(float Radius, float Height, int32 Segments);

    UFUNCTION(BlueprintCallable, Category = Defaults)
    void GenerateBellShape(float BaseRadius, float Height, float RimRadius, float CurvatureFactor, int32 NumSlices, int
                           NumStacks);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GenerateInvertedSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray
                                      <FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GenerateTorus(float InnerRadius, float OuterRadius, int32 RadialSegments, int32 TubularSegments);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GenerateCornShape(int32 NumSegments, float BaseRadiusX, float BaseRadiusY, float Height);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GeneratePyramid(float BaseLength, float Height);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GenerateFenceTop(float Radius, float Length, float Width);
    UFUNCTION(BlueprintCallable, Category = Defaults)
	void GenerateSemiEggShape(int32 NumSegments, float RadiusX, float RadiusY, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray
	                          <FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, TArray<FLinearColor>& VertexColors);
    UFUNCTION(BlueprintCallable, Category = Defaults)
    void BuildQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FProcMeshTangent>& InTangents, TArray<FVector2D>& InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FProcMeshTangent Tangent);

};