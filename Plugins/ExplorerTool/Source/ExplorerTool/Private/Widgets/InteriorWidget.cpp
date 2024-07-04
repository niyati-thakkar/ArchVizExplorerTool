// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteriorWidget.h"

// Fill out your copyright notice in the Description page of Project Settings.
#include "Widgets/ConstructionWidget.h"
#include "Game/ArchVizPlayerController.h"


void UInteriorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	// Bind button click events


	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());
	// Populate the scroll boxes with initial data
	PopulateScrollBoxes();
}

bool UInteriorWidget::Initialize()
{
	if (!Super::Initialize()) return false;


	if (DeleteButton)
	{
		DeleteButton->OnClicked.AddDynamic(this, &UInteriorWidget::HandleDeleteButtonClicked);
	}

	if (DeleteTitle)
	{
		DeleteTitle->SetTitleText(FText::FromString("Delete"));
		DeleteTitle->SetDesignBorderColor(FColor::Emerald);
	}

	if (WallPlaceables_SB)
	{
		WallPlaceables_SB->SetScrollBoxTitle(FText::FromString("Wall Placeables"), FColor::Blue);
	}
	if (CeilingPlaceables_SB)
	{
		CeilingPlaceables_SB->SetScrollBoxTitle(FText::FromString("Ceiling Placeables"), FColor::Green);
	}
	if (FloorPlaceables_SB)
	{
		FloorPlaceables_SB->SetScrollBoxTitle(FText::FromString("Floor Placeables"), FColor::Red);
	}
	return true;
}

void UInteriorWidget::HandleFloorItemClicked(FText Text)
{
	for (FInteriorItem FloorItem : DataAsset->FloorPlaceables)
	{
		if (FloorItem.ItemName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->InteriorManager->PlaceOnFloor(FloorItem.StaticMesh);
				break;
			}
		}
	}
	
}

void UInteriorWidget::HandleCeilingItemClicked(FText Text)
{
	for (FInteriorItem CeilingItem : DataAsset->CeilingPlaceables)
	{
		if (CeilingItem.ItemName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->InteriorManager->AttachToCeiling(CeilingItem.StaticMesh);
				break;
			}
		}
	}
}

void UInteriorWidget::HandleWallItemClicked(FText Text)
{
	for (FInteriorItem FloorItem : DataAsset->WallPlaceables)
	{
		if (FloorItem.ItemName.EqualTo(Text))
		{
			if (PlayerController)
			{
				PlayerController->InteriorManager->PlaceOnWall(FloorItem.StaticMesh);
				break;
			}
		}
	}
}

void UInteriorWidget::HandleDeleteButtonClicked()
{
	PlayerController->InteriorManager->DeleteButtonClicked();
}
void UInteriorWidget::PopulateScrollBoxes()
{
	if (DataAsset)
	{
		// Populate RoadState_SB
		for (FInteriorItem WallItem : DataAsset->WallPlaceables)
		{

			WallPlaceables_SB->AddScrollBoxElement(WallItem.ItemName, WallItem.MeshPreview);
			
		}

		for (FInteriorItem CeilingItem : DataAsset->CeilingPlaceables)
		{

			CeilingPlaceables_SB->AddScrollBoxElement(CeilingItem.ItemName, CeilingItem.MeshPreview);

		}

		for (FInteriorItem FloorItem : DataAsset->FloorPlaceables)
		{

			FloorPlaceables_SB->AddScrollBoxElement(FloorItem.ItemName, FloorItem.MeshPreview);

		}

	}

	//// Bind element click events
	WallPlaceables_SB->ElementSelected.BindUObject(this, &UInteriorWidget::HandleWallItemClicked);
	FloorPlaceables_SB->ElementSelected.BindUObject(this, &UInteriorWidget::HandleFloorItemClicked);
	CeilingPlaceables_SB->ElementSelected.BindUObject(this, &UInteriorWidget::HandleCeilingItemClicked);
	
}
