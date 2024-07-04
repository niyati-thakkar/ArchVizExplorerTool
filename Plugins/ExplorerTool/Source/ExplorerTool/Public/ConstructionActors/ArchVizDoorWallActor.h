// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "ArchVizDoorActor.h"
#include "ArchVizWallActor.h"
#include "GameFramework/Actor.h"
#include "ArchVizDoorWallActor.generated.h"

UCLASS()
class EXPLORERTOOL_API AArchVizDoorWallActor : public AArchVizConstructionActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AArchVizDoorWallActor();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    void SetMaterial(UMaterialInterface* Material);


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* WallComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    AArchVizDoorActor* DoorActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
    UStaticMesh* DoorWallMesh;


};

