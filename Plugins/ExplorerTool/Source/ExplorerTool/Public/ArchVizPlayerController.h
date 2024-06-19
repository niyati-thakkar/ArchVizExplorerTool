// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputcomponent.h"
#include "TaskBar.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RoadConstructionActor.h"
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
	TArray<ARoadConstructionActor*> RoadConstructionActors;

	UPROPERTY()
	TArray<FConstructedRoad>  RoadInfo;


	UPROPERTY()
	UArchVizSaveTool* SaveTool;

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTaskBar> TaskBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBar* TaskBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* TaskBarDataAsset;

	UFUNCTION()
	void WidgetSelected(FText SelectionName);

};
