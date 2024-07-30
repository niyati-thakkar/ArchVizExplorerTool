// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/RoadWidget.h"

#include "Game/ArchVizPlayerController.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Widgets/ScrollBoxElementWidget.h"
#include "TaskBarDataAsset.h"

void URoadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

	SetInputActionPriority(5);
	SetIsFocusable(true);

	RoadWidthSpinBox->SetValue(100);
	// Bind button click events
	
	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());

	// Populate the scroll boxes with initial data
	PopulateScrollBoxes();
}

bool URoadWidget::Initialize()
{
	if (!Super::Initialize()) return false;
	
	if (UndoButton)
	{
		UndoButton->OnClicked.AddDynamic(this, &URoadWidget::HandleUndoButtonClicked);
	}

	if (DeleteButton)
	{
		DeleteButton->OnClicked.AddDynamic(this, &URoadWidget::HandleDeleteButtonClicked);
	}
	if(RoadWidthSpinBox)
	{
		RoadWidthSpinBox->OnValueChanged.AddDynamic(this, &URoadWidget::HandleWidthChange);
	}
	if(RoadSizeTitle)
	{
		RoadSizeTitle->SetTitleText(FText::FromString("Road Width"));
		RoadSizeTitle->SetDesignBorderColor(FColor::Magenta);
	}
	if (DeleteTitle)
	{
		DeleteTitle->SetTitleText(FText::FromString("Delete"));
		DeleteTitle->SetDesignBorderColor(FColor::Emerald);
	}
	if (UndoTitle)
	{
		UndoTitle->SetTitleText(FText::FromString("Undo"));
		UndoTitle->SetDesignBorderColor(FColor::Cyan);
	}
	if (RoadMaterial_SB)
	{
		RoadMaterial_SB->SetScrollBoxTitle(FText::FromString("Materials"), FColor::Blue);
	}
	if (RoadState_SB)
	{
		RoadState_SB->SetScrollBoxTitle(FText::FromString("States"), FColor::Green);
	}
	if (RoadType_SB)
	{
		RoadType_SB->SetScrollBoxTitle(FText::FromString("Types"), FColor::Red);
	}
	return true;
}

void URoadWidget::HandleUndoButtonClicked()
{
	// Handle the undo button click event
	UE_LOG(LogTemp, Log, TEXT("Undo button clicked"));
	if (PlayerController)
	{
		PlayerController->RoadManager->RemoveLastSplinePoint();
	}
}
void URoadWidget::UpdateMode(ERoadState roadState)
{
	for (FRoadState RoadState : DataAsset->RoadStates)
	{
		if (RoadState.RoadState == (roadState))
		{
			RoadState_SB->SetButtonSelected(RoadState.RoadStateName);
			break;
		}
	}
}

void URoadWidget::HandleDeleteButtonClicked()
{
	// Handle the delete button click event
	UE_LOG(LogTemp, Log, TEXT("Delete button clicked"));
	if (PlayerController)
	{
		PlayerController->RoadManager->DeleteRoad();
	}
}

void URoadWidget::HandleRoadTypeElement(FText ElementText)
{
	// Handle element click event from scroll box elements
	UE_LOG(LogTemp, Log, TEXT("Element clicked: %s"), *ElementText.ToString());
	for(FRoadType RoadTypeElement: DataAsset->RoadTypes)
	{
		if(RoadTypeElement.RoadTypeName.EqualTo(ElementText))
		{
			if(PlayerController)
			{
				PlayerController->RoadManager->ChangeRoadType(RoadTypeElement.RoadType);
				break;
			}
		}
	}
	
}
void URoadWidget::HandleRoadStateElement(FText ElementText)
{
	// Handle element click event from scroll box elements
	UE_LOG(LogTemp, Log, TEXT("Element clicked: %s"), *ElementText.ToString());
	for (FRoadState RoadStateElement : DataAsset->RoadStates)
	{
		if (RoadStateElement.RoadStateName.EqualTo(ElementText))
		{
			if (PlayerController)
			{
				PlayerController->RoadManager->ChangeRoadState(RoadStateElement.RoadState);
				break;
			}
		}
	}
}

void URoadWidget::HandleRoadMaterialElement(FText ElementText)
{
	// Handle element click event from scroll box elements
	UE_LOG(LogTemp, Log, TEXT("Element clicked: %s"), *ElementText.ToString());
	for (FRoadMaterial RoadMaterialElement : DataAsset->RoadMaterials)
	{
		if (RoadMaterialElement.RoadMaterialName.EqualTo(ElementText))
		{
			if (PlayerController)
			{
				PlayerController->RoadManager->ChangeRoadMaterial(RoadMaterialElement.RoadMaterial);
				break;
			}
		}
	}
}

void URoadWidget::HandleWidthChange(float RoadWidth)
{
	if(PlayerController)
	{
		PlayerController->RoadManager->ChangeRoadWidth(RoadWidth);
	}
}


void URoadWidget::PopulateScrollBoxes()
{
	// Example data asset initialization
	if (DataAsset)
	{

		// Populate RoadState_SB
		for (const FRoadState& RoadState : DataAsset->RoadStates)
		{
			if (RoadState_SB)
			{
				RoadState_SB->AddScrollBoxElement(RoadState.RoadStateName, RoadState.RoadStatePreview);
			}
		}

		// Populate RoadType_SB
		for (const FRoadType& RoadType : DataAsset->RoadTypes)
		{
			if (RoadType_SB)
			{
				RoadType_SB->AddScrollBoxElement(RoadType.RoadTypeName, RoadType.RoadTypePreview);
			}
		}

		// Populate RoadMaterial_SB
		for (const FRoadMaterial& RoadMaterial : DataAsset->RoadMaterials)
		{
			if (RoadMaterial_SB)
			{
				RoadMaterial_SB->AddScrollBoxElement(RoadMaterial.RoadMaterialName, RoadMaterial.RoadMaterialPreview);
			}
		}
	}

	// Bind element click events
	RoadState_SB->ElementSelected.BindUObject(this, &URoadWidget::HandleRoadStateElement);
	RoadType_SB->ElementSelected.BindUObject(this, &URoadWidget::HandleRoadTypeElement);
	RoadMaterial_SB->ElementSelected.BindUObject(this, &URoadWidget::HandleRoadMaterialElement);
	
	

}
void URoadWidget::UpdateWidth()
{
	if(PlayerController)
	{
		PlayerController->RoadManager->ChangeRoadWidth(RoadWidthSpinBox->GetValue());
	}
}

void URoadWidget::UpdatePropertiesSelected(UMaterialInterface* RoadMaterial, ERoadType Road, float RoadWidth)
{
	for (FRoadMaterial roadMaterial : DataAsset->RoadMaterials)
	{
		if ((roadMaterial.RoadMaterial) == (RoadMaterial))
		{
			RoadMaterial_SB->SetButtonSelected(roadMaterial.RoadMaterialName);
			break;
		}
	}
	for (FRoadType RoadType : DataAsset->RoadTypes)
	{
		if (RoadType.RoadType == (Road))
		{
			RoadType_SB->SetButtonSelected(RoadType.RoadTypeName);
			break;
		}
	}
	RoadWidthSpinBox->SetValue(RoadWidth);
}
