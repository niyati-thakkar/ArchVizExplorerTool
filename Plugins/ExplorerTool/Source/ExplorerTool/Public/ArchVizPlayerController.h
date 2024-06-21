// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizSaveTool.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputcomponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ArchVizRoadActor.h"
#include "TaskBarDataAsset.h"
#include "ArchVizPlayerController.generated.h"


/**
 * 
 */
class UArchVizSaveTool;

UCLASS()
class EXPLORERTOOL_API AArchVizPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	AArchVizPlayerController();

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	void AddRoadConstructionMappings();
	void GetMouseClickLocation(const FInputActionValue& InputAction);
	void GenerateRoadPoint(FHitResult HitResult);
	void SaveSlot();
	void LoadSlot();
	

public:
	
	UPROPERTY()
	TArray<AArchVizRoadActor*> RoadConstructionActors;

	UPROPERTY()
	TArray<FConstructedRoad>  RoadInfo;

	UPROPERTY()
	UArchVizSaveTool* SaveTool;

	UPROPERTY()
	AArchVizActor* CurrentSelectedActor;

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* TaskBarDataAsset;

	UFUNCTION(BlueprintCallable)
	void ChangeRoadType(ERoadType RoadType);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadState(ERoadState RoadState);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadWidth(int RoadWidth);
	UFUNCTION(BlueprintCallable)
	void ChangeRoadMaterial(UMaterialInterface* NewMaterial);
	UFUNCTION(BlueprintCallable)
	void DeleteRoad();
	UFUNCTION(BlueprintCallable)
	void RemoveLastSplinePoint();
};
