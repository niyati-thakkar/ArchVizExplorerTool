// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstructionActors/ArchVizGridActor.h"
#include "SaveLoad/ArchVizSaveTool.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputcomponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "ConstructionActors/ArchVizFollowArrowActor.h"
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
	void HandleArrowSelection(const FHitResult& HitResult);
	void HighlightSelectedActor();
	void UnhighlightDeselectedActor();
	void GetMouseClickLocation(const FInputActionValue& InputAction);
	UArchVizManager* GetManager();

	//void SaveSlot();
	//void LoadSlot();
	

public:
	
	

	UPROPERTY()
	UArchVizSaveTool* SaveTool;

	UPROPERTY()
	AArchVizActor* CurrentSelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* TaskBarDataAsset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AArchVizFollowArrowActor> FollowArrowActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArchVizFollowArrowActor* FollowArrowActor;
	FVector LastHitLocation;

	
	void UpdateArrowPosition();
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void ChangeMode(EArchVizMode SetMode);
	EArchVizMode GetCurrentMode();
	void Save();
	void Load();
	UFUNCTION(BlueprintCallable)
	void ArrowSelected();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsArrowSelected = false;

	UPROPERTY()
	AArchVizGridActor* GridActor;
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
