// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizConstructionActor.h"
#include "ArchVizWallActor.h"
#include "GameFramework/Actor.h"
#include "ArchVizDoorActor.generated.h"

UCLASS()
class EXPLORERTOOL_API AArchVizDoorActor : public AArchVizConstructionActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	AArchVizDoorActor();

	UStaticMeshComponent* DoorFrameComponent;

	UStaticMeshComponent* DoorComponent;

	UStaticMesh* DoorFrameMesh;
	UStaticMesh* DoorMesh;
	AArchVizWallActor* WallActor;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	AArchVizWallActor* GetWallActor();
	void RotateActor(FRotator ApplyRotation) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
