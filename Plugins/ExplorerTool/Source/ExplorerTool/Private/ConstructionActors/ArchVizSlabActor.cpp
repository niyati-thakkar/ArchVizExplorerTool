// Fill out your copyright notice in the Description page of Project Settings.



#include "ConstructionActors/ArchVizSlabActor.h"

AArchVizSlabActor::AArchVizSlabActor()
{

	SlabComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlabComponent"));
	RootComponent = SlabComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SlabMeshAsset(TEXT("/Game/StarterContent/Architecture/Floor_400x400.Floor_400x400"));
	if (SlabMeshAsset.Succeeded())
	{
		SlabMesh = SlabMeshAsset.Object;
		SlabComponent->SetStaticMesh(SlabMesh);
	}
}
