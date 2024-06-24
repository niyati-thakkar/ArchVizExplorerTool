// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadWidget.h"

#include "ArchVizPlayerController.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ScrollBoxElementWidget.h"
#include "TaskBarDataAsset.h"

void URoadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());

	SetInputActionPriority(5);
	SetIsFocusable(true);

	RoadWidthSpinBox->SetValue(100);
	// Bind button click events
	
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
	
	return true;
}

void URoadWidget::HandleUndoButtonClicked()
{
	// Handle the undo button click event
	UE_LOG(LogTemp, Log, TEXT("Undo button clicked"));
	if (PlayerController)
	{
		PlayerController->RemoveLastSplinePoint();
	}
}

void URoadWidget::HandleDeleteButtonClicked()
{
	// Handle the delete button click event
	UE_LOG(LogTemp, Log, TEXT("Delete button clicked"));
	if (PlayerController)
	{
		PlayerController->DeleteRoad();
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
				PlayerController->ChangeRoadType(RoadTypeElement.RoadType);
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
				PlayerController->ChangeRoadState(RoadStateElement.RoadState);
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
				PlayerController->ChangeRoadMaterial(RoadMaterialElement.RoadMaterial);
			}
		}
	}
}

void URoadWidget::HandleElementClicked(FText ElementText)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(ElementText.ToString()));
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
	RoadMaterial_SB->AddToViewport();
	

}
void URoadWidget::UpdateWidth()
{
	if(PlayerController)
	{
		PlayerController->ChangeRoadWidth(RoadWidthSpinBox->GetValue());
	}
}