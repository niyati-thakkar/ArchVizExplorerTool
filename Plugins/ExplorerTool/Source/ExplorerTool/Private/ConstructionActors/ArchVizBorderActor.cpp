// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionActors/ArchVizBorderActor.h"

// Sets default values
AArchVizBorderActor::AArchVizBorderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (CylinderMeshAsset.Succeeded())
	{
		StaticMesh = CylinderMeshAsset.Object;
	}
	StaticMeshComponent->SetStaticMesh(StaticMesh);
	StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, 90.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(0.5,0.5,10));
}

// Called when the game starts or when spawned
void AArchVizBorderActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArchVizBorderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

