// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizSaveTool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArchVizGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UArchVizGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

private:
	UPROPERTY()
	FString CurrentSaveGameSlot;

	UPROPERTY()
	UArchVizSaveTool* CurrentSaveGame;
public:
	UFUNCTION()
	UArchVizSaveTool* GetCurrentSaveGameSlot()
	{
		return CurrentSaveGame;
	}

	UFUNCTION()
	FString GetCurrentSaveGameSlotName()
	{
		return CurrentSaveGameSlot;
	}

	UFUNCTION()
	void CreateSaveGameSlot(const FString& SlotName);

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame(const FString& SlotName);

	UFUNCTION()
	TArray<FString> GetAllSaveSlots() const;


	UPROPERTY()
	TArray<FString> SaveSlots;

};
