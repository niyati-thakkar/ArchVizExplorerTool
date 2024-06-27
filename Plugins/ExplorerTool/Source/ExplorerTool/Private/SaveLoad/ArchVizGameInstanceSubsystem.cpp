#include "SaveLoad/ArchVizGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UArchVizGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Load all existing save slots on initialization
    TArray<FString> SlotNames = GetAllSaveSlots();
    for (const FString& SlotName : SlotNames)
    {
        if (!SaveSlots.Contains(SlotName))
        {
            SaveSlots.Add(SlotName);
        }
    }

    // If the default save slot does not exist, create it
    if (!CurrentSaveGame || !CurrentSaveGame->GameSlots.Contains("default"))
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
    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<UArchVizSaveTool>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveTool::StaticClass()));
    }

    CurrentSaveGame->GameSlots.Add(SlotName, FSaveSlotElement());
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
    if (UArchVizSaveTool* LoadedSaveGame = Cast<UArchVizSaveTool>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
    {
        CurrentSaveGame = LoadedSaveGame;
    }
    else
    {
        CurrentSaveGame = Cast<UArchVizSaveTool>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveTool::StaticClass()));
    }

    if (CurrentSaveGame->GameSlots.Contains(SlotName))
    {
        CurrentSaveGameSlot = SlotName;
    }
}

TArray<FString> UArchVizGameInstanceSubsystem::GetAllSaveSlots() const
{
    TArray<FString> SlotNames;

    if (CurrentSaveGame)
    {
        SlotNames.Reserve(CurrentSaveGame->GameSlots.Num());

        for (const auto& Slot : CurrentSaveGame->GameSlots)
        {
            SlotNames.Add(Slot.Key);
        }
    }

    return SlotNames;
}
