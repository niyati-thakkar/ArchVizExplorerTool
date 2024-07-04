// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveLoad/ArchVizSaveTool.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputcomponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "Managers/ArchVizConstructionManager.h"
#include "Managers/ArchVizInteriorManager.h"
#include "Managers/ArchVizRoadManager.h"
#include "Managers/ArchVizSaveLoadManager.h"

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
	
	void AddMappings();
	void GetMouseClickLocation(const FInputActionValue& InputAction);
	void ApplyRotation(const FInputActionValue& InputAction);

	UArchVizManager* GetManager();

	//void SaveSlot();
	//void LoadSlot();
	

public:
	void Save();
	FHitResult GetMouseLocation(const TArray<AActor*>& IgnoredActors);

	UPROPERTY()
	UArchVizSaveTool* SaveTool;

	UPROPERTY()
	AArchVizActor* CurrentSelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* TaskBarDataAsset;



	FVector LastHitLocation;
	void Load();

	//void UpdateArrowPosition();
	void Tick(float DeltaTime);

	TArray<FString> FindFiles(FString Path, FString Extension);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MasterWidget;

	UFUNCTION(BlueprintCallable)
	void ChangeMode(EArchVizMode SetMode);
	EArchVizMode GetCurrentMode();
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsArrowSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArchVizMode CurrentMode = EArchVizMode::None;

	UPROPERTY()
	UArchVizRoadManager* RoadManager;

	UPROPERTY()
	UArchVizConstructionManager* ConstructionManager;

	UPROPERTY()
	UArchVizInteriorManager* InteriorManager;

	UPROPERTY()
	UArchVizSaveLoadManager* SaveLoadManager;

};
