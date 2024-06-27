// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/ArchVizMasterWidget.h"



void UArchVizMasterWidget::ChangeToRoadMode()
{
    if (PlayerController->GetCurrentMode() == EArchVizMode::RoadMode)
    {
        PlayerController->ChangeMode(EArchVizMode::None);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        RemoveButtonStyle();
        PlayerController->ChangeMode(EArchVizMode::RoadMode);
        WidgetSwitcher->SetActiveWidget(RoadModeWidget);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
        RoadModeButton->Button->SetStyle(ButtonStyleClicked);
    }
}

void UArchVizMasterWidget::ChangeToConstructionMode()
{
    if (PlayerController->GetCurrentMode() == EArchVizMode::ConstructionMode)
    {
        PlayerController->ChangeMode(EArchVizMode::None);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        RemoveButtonStyle();
        PlayerController->ChangeMode(EArchVizMode::ConstructionMode);
        WidgetSwitcher->SetActiveWidget(ConstructionModeWidget);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
        ConstructionModeButton->Button->SetStyle(ButtonStyleClicked);
    }
}

void UArchVizMasterWidget::ChangeToInteriorMode()
{
    if (PlayerController->GetCurrentMode() == EArchVizMode::InteriorMode)
    {
        PlayerController->ChangeMode(EArchVizMode::None);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        RemoveButtonStyle();
        PlayerController->ChangeMode(EArchVizMode::InteriorMode);
        WidgetSwitcher->SetActiveWidget(InteriorModeWidget);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
        InteriorModeButton->Button->SetStyle(ButtonStyleClicked);
    }
}

void UArchVizMasterWidget::ChangeToSaveLoadMode()
{
    if (PlayerController->GetCurrentMode() == EArchVizMode::SaveLoadMode)
    {
        PlayerController->ChangeMode(EArchVizMode::None);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        RemoveButtonStyle();
        PlayerController->ChangeMode(EArchVizMode::SaveLoadMode);
        WidgetSwitcher->SetActiveWidget(SaveLoadModeWidget);
        WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
        SaveLoadModeButton->Button->SetStyle(ButtonStyleClicked);
    }
}


void UArchVizMasterWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());
    WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
    if (MasterWidgetButtonClass)
    {
        RoadModeButton = CreateWidget<UMasterWidgetButton>(this, MasterWidgetButtonClass);
        ConstructionModeButton = CreateWidget<UMasterWidgetButton>(this, MasterWidgetButtonClass);
        InteriorModeButton = CreateWidget<UMasterWidgetButton>(this, MasterWidgetButtonClass);
        SaveLoadModeButton = CreateWidget<UMasterWidgetButton>(this, MasterWidgetButtonClass);

        RoadModeButton->Button->SetStyle(ButtonStyleTaskbar);
        ConstructionModeButton->Button->SetStyle(ButtonStyleTaskbar);
        InteriorModeButton->Button->SetStyle(ButtonStyleTaskbar);
        SaveLoadModeButton->Button->SetStyle(ButtonStyleTaskbar);

        TaskBarButtonBox->ClearChildren();
        WidgetSwitcher->ClearChildren();

        TaskBarButtonBox->AddChild(RoadModeButton);
        TaskBarButtonBox->AddChild(ConstructionModeButton);
        TaskBarButtonBox->AddChild(InteriorModeButton);
        TaskBarButtonBox->AddChild(SaveLoadModeButton);
    }

    if (RoadWidgetClass)
    {
        RoadModeWidget = CreateWidget<URoadWidget>(this, RoadWidgetClass);
        WidgetSwitcher->AddChild(RoadModeWidget);
    }

    if (ConstructionWidgetClass)
    {
        ConstructionModeWidget = CreateWidget<UConstructionWidget>(this, ConstructionWidgetClass);
        WidgetSwitcher->AddChild(ConstructionModeWidget);
    }

    if (InteriorWidgetClass)
    {
        InteriorModeWidget = CreateWidget<UInteriorWidget>(this, InteriorWidgetClass);
        WidgetSwitcher->AddChild(InteriorModeWidget);
    }

    if (SaveLoadWidgetClass)
    {
        SaveLoadModeWidget = CreateWidget<USaveLoadWidget>(this, SaveLoadWidgetClass);
        WidgetSwitcher->AddChild(SaveLoadModeWidget);
    }

    if (RoadModeButton)
    {
        RoadModeButton->Button->OnClicked.AddDynamic(this, &UArchVizMasterWidget::ChangeToRoadMode);
        if (DataAsset)
        {
            FMasterWidgetButtonProp* ButtonProp = DataAsset->MasterWidgetContentMap.Find(EArchVizMode::RoadMode);
            if (ButtonProp)
            {
                RoadModeButton->SetButtonProperties(ButtonProp->ButtonText, ButtonProp->ButtonPreview, EArchVizMode::RoadMode);
            }
        }
    }

    if (ConstructionModeButton)
    {
        ConstructionModeButton->Button->OnClicked.AddDynamic(this, &UArchVizMasterWidget::ChangeToConstructionMode);
        if (DataAsset)
        {
            FMasterWidgetButtonProp* ButtonProp = DataAsset->MasterWidgetContentMap.Find(EArchVizMode::ConstructionMode);
            if (ButtonProp)
            {
                ConstructionModeButton->SetButtonProperties(ButtonProp->ButtonText, ButtonProp->ButtonPreview, EArchVizMode::ConstructionMode);
            }
        }
    }

    if (InteriorModeButton)
    {
        InteriorModeButton->Button->OnClicked.AddDynamic(this, &UArchVizMasterWidget::ChangeToInteriorMode);
        if (DataAsset)
        {
            FMasterWidgetButtonProp* ButtonProp = DataAsset->MasterWidgetContentMap.Find(EArchVizMode::InteriorMode);
            if (ButtonProp)
            {
                InteriorModeButton->SetButtonProperties(ButtonProp->ButtonText, ButtonProp->ButtonPreview, EArchVizMode::InteriorMode);
            }
        }
    }

    if (SaveLoadModeButton)
    {
        SaveLoadModeButton->Button->OnClicked.AddDynamic(this, &UArchVizMasterWidget::ChangeToSaveLoadMode);
        if (DataAsset)
        {
            FMasterWidgetButtonProp* ButtonProp = DataAsset->MasterWidgetContentMap.Find(EArchVizMode::SaveLoadMode);
            if (ButtonProp)
            {
                SaveLoadModeButton->SetButtonProperties(ButtonProp->ButtonText, ButtonProp->ButtonPreview, EArchVizMode::SaveLoadMode);
            }
        }
    }
}

void UArchVizMasterWidget::RemoveButtonStyle()
{
    if (RoadModeButton)
    {
        RoadModeButton->Button->SetStyle(ButtonStyleTaskbar);
    }
    if (ConstructionModeButton)
    {
        ConstructionModeButton->Button->SetStyle(ButtonStyleTaskbar);
    }
    if (InteriorModeButton)
    {
        InteriorModeButton->Button->SetStyle(ButtonStyleTaskbar);
    }
    if (SaveLoadModeButton)
    {
        SaveLoadModeButton->Button->SetStyle(ButtonStyleTaskbar);
    }
}
