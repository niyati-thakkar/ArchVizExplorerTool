// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionActors/ArchVizFollowArrowActor.h"

// Sets default values
AArchVizFollowArrowActor::AArchVizFollowArrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	ArrowMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ArrowMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AArchVizFollowArrowActor::BeginPlay()
{
	Super::BeginPlay();
	if (ArrowMesh)
	{
		ArrowMeshComponent->SetStaticMesh(ArrowMesh);
		if (MaterialInterface)
		{
			/*for (int32 i : ArrowMeshComponent->GetNumMaterials())
			{
				ArrowMeshComponent->SetMaterial(i, MaterialInterface);
			}*/
		}
	}
	
}

// Called every frame
void AArchVizFollowArrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

