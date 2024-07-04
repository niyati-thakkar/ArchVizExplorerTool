// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionActors/ArchVizFloorActor.h"

// Sets default values
AArchVizFloorActor::AArchVizFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArchVizFloorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArchVizFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
}

