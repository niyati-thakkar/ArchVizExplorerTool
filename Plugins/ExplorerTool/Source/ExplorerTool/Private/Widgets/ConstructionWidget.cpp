// Fill out your copyright notice in the Description page of Project Settings.
#include "Widgets/ConstructionWidget.h"
#include "Game/ArchVizPlayerController.h"


void UConstructionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	// Bind button click events


	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());
	// Populate the scroll boxes with initial data
	PopulateScrollBoxes();
}

bool UConstructionWidget::Initialize()
{
	if (!Super::Initialize()) return false;


	if (DeleteButton)
	{
		DeleteButton->OnClicked.AddDynamic(this, &UConstructionWidget::HandleDeleteButtonClicked);
	}

	if (DeleteTitle)
	{
		DeleteTitle->SetTitleText(FText::FromString("Delete"));
		DeleteTitle->SetDesignBorderColor(FColor::Emerald);
	}

	if (ConstructionElements_SB)
	{
		ConstructionElements_SB->SetScrollBoxTitle(FText::FromString("Elements"), FColor::Blue);
	}
	if (WallMaterial_SB)
	{
		WallMaterial_SB->SetScrollBoxTitle(FText::FromString("Wall Materials"), FColor::Green);
	}
	if (FloorMaterial_SB)
	{
		FloorMaterial_SB->SetScrollBoxTitle(FText::FromString("Floor Materials"), FColor::Red);
	}
	if (RoofMaterial_SB)
	{
		RoofMaterial_SB->SetScrollBoxTitle(FText::FromString("Roof Materials"), FColor::Yellow);
	}
	return true;
}

void UConstructionWidget::HandleConstructionElementChange(FText Text)
{
	for (FConstructionElement ConstructionElement : DataAsset->ConstructionElements)
	{
		if (ConstructionElement.ConstructionElementName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->ConstructionManager->ChangeConstructionType(ConstructionElement.ConstructionType);
				break;
			}
		}
	}
}

void UConstructionWidget::HandleWallMaterialChange(FText Text)
{
	for (FConstructionWallMaterial WallMaterial : DataAsset->ConstructionWallMaterials)
	{
		if (WallMaterial.WallMaterialName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->ConstructionManager->ChangeWallMaterial(WallMaterial.WallMaterial);
				break;
			}
		}
	}
}

void UConstructionWidget::HandleFloorMaterialChange(FText Text)
{
	for (FConstructionFloorMaterial SlabMaterial : DataAsset->ConstructionFloorMaterials)
	{
		if (SlabMaterial.FloorMaterialName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->ConstructionManager->ChangeFloorMaterial(SlabMaterial.FloorMaterial);
				break;
			}
		}
	}
}
void UConstructionWidget::HandleRoofMaterialChange(FText Text)
{
	for (FConstructionRoofMaterial SlabMaterial : DataAsset->ConstructionRoofMaterials)
	{
		if (SlabMaterial.RoofMaterialName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->ConstructionManager->ChangeRoofMaterial(SlabMaterial.RoofMaterial);
				break;
			}
		}
	}
}
void UConstructionWidget::HandleDeleteButtonClicked()
{
	PlayerController->ConstructionManager->DeleteButtonClicked();
}

void UConstructionWidget::ClearSelected()
{
	FloorMaterial_SB->RemoveSelection();
	RoofMaterial_SB->RemoveSelection();
	WallMaterial_SB->RemoveSelection();
	ConstructionElements_SB->RemoveSelection();
}

void UConstructionWidget::PopulateScrollBoxes()
{
	if (DataAsset)
	{

		// Populate RoadState_SB
		for (const FConstructionWallMaterial& WallMaterial : DataAsset->ConstructionWallMaterials)
		{
			if (WallMaterial_SB)
			{
				WallMaterial_SB->AddScrollBoxElement(WallMaterial.WallMaterialName, WallMaterial.WallMaterialPreview);
			}
		}

		// Populate RoadType_SB
		for (const FConstructionFloorMaterial& SlabMaterial : DataAsset->ConstructionFloorMaterials)
		{
			if (FloorMaterial_SB)
			{
				FloorMaterial_SB->AddScrollBoxElement(SlabMaterial.FloorMaterialName, SlabMaterial.FloorMaterialPreview);
			}
		}
		for (const FConstructionRoofMaterial& SlabMaterial : DataAsset->ConstructionRoofMaterials)
		{
			if (RoofMaterial_SB)
			{
				RoofMaterial_SB->AddScrollBoxElement(SlabMaterial.RoofMaterialName, SlabMaterial.RoofMaterialPreview);
			}
		}

		// Populate RoadMaterial_SB
		for (const FConstructionElement& ElementProp : DataAsset->ConstructionElements)
		{

			if (ConstructionElements_SB)
			{
				ConstructionElements_SB->AddScrollBoxElement(ElementProp.ConstructionElementName, ElementProp.ElementPreview);
			}
		}

	}

	//// Bind element click events
	ConstructionElements_SB->ElementSelected.BindUObject(this, &UConstructionWidget::HandleConstructionElementChange);
	FloorMaterial_SB->ElementSelected.BindUObject(this, &UConstructionWidget::HandleFloorMaterialChange);
	RoofMaterial_SB->ElementSelected.BindUObject(this, &UConstructionWidget::HandleRoofMaterialChange);
	WallMaterial_SB->ElementSelected.BindUObject(this, &UConstructionWidget::HandleWallMaterialChange);
	//RoadMaterial_SB->AddToViewport();
}
void UConstructionWidget::UpdatePropertiesSelected(UMaterialInterface* Material)
{
	ClearSelected();
	for (FConstructionWallMaterial material : DataAsset->ConstructionWallMaterials)
	{
		if ((material.WallMaterial) == (Material))
		{
			WallMaterial_SB->SetButtonSelected(material.WallMaterialName);
			break;
		}
	}
}
void UConstructionWidget::UpdatePropertiesSelected(UMaterialInterface* FloorMaterial, UMaterialInterface* RoofMaterial)
{
	
	ClearSelected();
	for (FConstructionFloorMaterial material : DataAsset->ConstructionFloorMaterials)
	{
		if ((material.FloorMaterial) == (FloorMaterial))
		{
			FloorMaterial_SB->SetButtonSelected(material.FloorMaterialName);
			break;
		}
	}
	for (FConstructionRoofMaterial material : DataAsset->ConstructionRoofMaterials)
	{
		if ((material.RoofMaterial) == (RoofMaterial))
		{
			RoofMaterial_SB->SetButtonSelected(material.RoofMaterialName);
			break;
		}
	}
}