// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActor.h"

// Sets default values
AArchVizActor::AArchVizActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArchVizActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArchVizActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArchVizActor::HighlightSelectedActor() {

    TSet<UActorComponent*> ActorComponents = GetComponents();

    for (auto& ActorComponent : ActorComponents) {

        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {

            Component->SetRenderCustomDepth(true);

            Component->CustomDepthStencilValue = 2;

        }

    }

}

void AArchVizActor::UnhighlightDeselectedActor() {

    TSet<UActorComponent*> ActorComponents = GetComponents();

    for (auto& ActorComponent : ActorComponents) {

        if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {

            Component->SetRenderCustomDepth(false);

        }

    }

}

bool AArchVizActor::IsRotateable()
{
    return false;
}
void AArchVizActor::RotateActor(FRotator ApplyRotation)
{
    
	
};