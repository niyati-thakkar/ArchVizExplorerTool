// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActor.h"
#include "GameFramework/Actor.h"
#include "ArchVizFollowArrowActor.generated.h"

UCLASS()
class EXPLORERTOOL_API AArchVizFollowArrowActor : public AArchVizActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArchVizFollowArrowActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ArrowMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ArrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MaterialInterface;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
