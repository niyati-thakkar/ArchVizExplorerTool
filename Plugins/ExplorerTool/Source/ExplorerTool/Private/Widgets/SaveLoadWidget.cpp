// Fill out your copyright notice in the Description page of Project Settings.
#include "Widgets/SaveLoadWidget.h"
#include "Game/ArchVizPlayerController.h"


void USaveLoadWidget::HandleSaveButtonClicked(FText Text)
{
    if (PlayerController)
    {
        PlayerController->Save();
    }
}

void USaveLoadWidget::PopulateScrollBoxes()
{
    // Define the path to the SaveGames directory
    FString SaveDirectory = FPaths::ProjectSavedDir() / TEXT("SaveGames");

    // Find all .sav files in the SaveGames directory
    TArray<FString> SlotsAvailable = PlayerController->FindFiles(SaveDirectory, TEXT("*.sav"));
    ExistingProjects_SB->ClearAllChildren();
    // Now you can populate your scroll box with the found slots
    for (FString GameSlot : SlotsAvailable)
    {
        if (ExistingProjects_SB)
        {
            GameSlot = GameSlot.LeftChop(4);
            ExistingProjects_SB->AddScrollBoxElement(FText::FromString(GameSlot), DataAsset->SavedSlotsLogo);
        }
    }
    ExistingProjects_SB->ElementSelected.BindUObject(this, &USaveLoadWidget::UpdateCurrentSlot);
}

void USaveLoadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());

	// Populate the scroll boxes with initial data
	PopulateScrollBoxes();
}

void USaveLoadWidget::HandleLoadButtonClicked(FText Text)
{
    if (PlayerController)
    {
        PlayerController->Load();
    }
}

void USaveLoadWidget::HandleNewButtonClicked(FText Text)
{
    if (PlayerController)
    {
        FText ItemText = NewItemTextBox->GetText();

        if (!ItemText.IsEmpty())
        {
            PlayerController->SaveLoadManager->CreateSlotIfNotExists(ItemText.ToString());
        }
    }
    PopulateScrollBoxes();
}


bool USaveLoadWidget::Initialize()
{
	Super::Initialize();
    LoadButton = CreateWidget<UScrollBoxElementWidget>(this, ScrollBoxElementClass);
    SaveButton = CreateWidget<UScrollBoxElementWidget>(this, ScrollBoxElementClass);
    NewButton = CreateWidget<UScrollBoxElementWidget>(this, ScrollBoxElementClass);
    if (LoadButton)
    {
        LoadButton->OnClickedDelegate.BindUObject(this, &USaveLoadWidget::HandleLoadButtonClicked);
        LoadButton->SetElementNameAndIcon(FText::FromString("Load Game"), DataAsset->LoadSlotIcon);
    }

    if (SaveButton)
    {
        SaveButton->OnClickedDelegate.BindUObject(this, &USaveLoadWidget::HandleSaveButtonClicked);
        SaveButton->SetElementNameAndIcon(FText::FromString("Save Game"), DataAsset->SaveSlotIcon);
    }
    if (NewButton)
    {
        NewButton->OnClickedDelegate.BindUObject(this, &USaveLoadWidget::HandleNewButtonClicked);
        NewButton->SetElementNameAndIcon(FText::FromString("New Game"), DataAsset->NewSlotIcon);
    }
    ButtonBox->ClearChildren();
    NewButtonBox->ClearChildren();
    NewButtonBox->AddChild(NewItemTextBox);
    NewButtonBox->AddChild(NewButton);
    ButtonBox->AddChild(LoadButton);
    ButtonBox->AddChild(SaveButton);
    ButtonBox->AddChild(NewButtonBox);
    
    return true;
}


void USaveLoadWidget::UpdateCurrentSlot(FText SlotName)
{
    if (PlayerController)
    {
        PlayerController->SaveLoadManager->SetSlotName((SlotName).ToString());
    }
}