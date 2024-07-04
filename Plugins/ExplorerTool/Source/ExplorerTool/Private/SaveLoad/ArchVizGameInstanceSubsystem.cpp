// ArchVizGameInstanceSubsystem.cpp

#include "SaveLoad/ArchVizGameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UArchVizGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Load all existing save slots on initialization
    SaveSlots = GetAllSaveSlots();

    // If the default save slot does not exist, create it
    if (!SaveSlots.Contains("default"))
    {
        CreateSaveGameSlot("default");
        SaveGame();
    }
}

void UArchVizGameInstanceSubsystem::Deinitialize()
{
    Super::Deinitialize();

    // Save the list of save slots on deinitialization
    SaveGame();
}

void UArchVizGameInstanceSubsystem::CreateSaveGameSlot(const FString& SlotName)
{
    CurrentSaveGame = Cast<UArchVizSaveTool>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveTool::StaticClass()));
    SaveSlots.Add(SlotName); // Update the list of save slots
    CurrentSaveGameSlot = SlotName;
}

void UArchVizGameInstanceSubsystem::SaveGame()
{
    if (CurrentSaveGame && !CurrentSaveGameSlot.IsEmpty())
    {
        UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSaveGameSlot, 0);
    }
}

void UArchVizGameInstanceSubsystem::LoadGame(const FString& SlotName)
{
    CurrentSaveGame = Cast<UArchVizSaveTool>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (CurrentSaveGame)
    {
        CurrentSaveGameSlot = SlotName;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load game from slot %s"), *SlotName);
    }
}

TArray<FString> UArchVizGameInstanceSubsystem::GetAllSaveSlots() const
{
    TArray<FString> SlotNames;
    IFileManager::Get().FindFiles(SlotNames, *FPaths::ProjectSavedDir(), *FString::Printf(TEXT("%s.sav"), *CurrentSaveGameSlot));
    for (auto& SaveName : SlotNames)
    {
        SaveName = FPaths::GetBaseFilename(SaveName);
    }
    return SlotNames;
}
