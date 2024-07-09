#include "Widgets/ExteriorWidget.h"

#include "ExteriorActors/StaircaseActor.h"
#include "Game/ArchVizPlayerController.h"



void UExteriorWidget::NativeConstruct()
{
	Super::NativeConstruct();


	SetInputActionPriority(5);
	SetIsFocusable(true);

	NumberOfStairsSpinBox->SetValue(10);
	StairsLengthSpinBox->SetValue(10);
	StairsWidthSpinBox->SetValue(20);
	StairsDepthSpinBox->SetValue(3);
	// Bind button click events

	PlayerController = Cast<AArchVizPlayerController>(GetWorld()->GetFirstPlayerController());

	// Populate the scroll boxes with initial data
	PopulateScrollBoxes();
}

bool UExteriorWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (NumberOfStairsSpinBox)
	{
		NumberOfStairsSpinBox->OnValueChanged.AddDynamic(this, &UExteriorWidget::HandleNumberOfStairsChanged);
	}

	if (StairsDepthSpinBox)
	{
		StairsDepthSpinBox->OnValueChanged.AddDynamic(this, &UExteriorWidget::HandleStairsDepthChanged);
	}
	if (StairsLengthSpinBox)
	{
		StairsLengthSpinBox->OnValueChanged.AddDynamic(this, &UExteriorWidget::HandleStairsLengthChanged);
	}
	if (StairsWidthSpinBox)
	{
		StairsWidthSpinBox->OnValueChanged.AddDynamic(this, &UExteriorWidget::HandleStairsWidthChanged);
	}
	if (DeleteButton)
	{
		DeleteButton->OnClicked.AddDynamic(this, &UExteriorWidget::HandleDeleteButtonClicked);
	}
	if (DeleteTitle)
	{
		DeleteTitle->SetTitleText(FText::FromString("Delete"));
		DeleteTitle->SetDesignBorderColor(FColor::Emerald);
	}
	if (NumberOfStairsTitle)
	{
		NumberOfStairsTitle->SetTitleText(FText::FromString("Number of Stairs"));
		NumberOfStairsTitle->SetDesignBorderColor(FColor::Cyan);
	}
	if (StairsDepthTitle)
	{
		StairsDepthTitle->SetTitleText(FText::FromString("Stairs Depth"));
		StairsDepthTitle->SetDesignBorderColor(FColor::Red);
	}
	if (StairsWidthTitle)
	{
		StairsWidthTitle->SetTitleText(FText::FromString("Stairs Width"));
		StairsWidthTitle->SetDesignBorderColor(FColor::Yellow);
	}
	if (StairsLengthTitle)
	{
		StairsLengthTitle->SetTitleText(FText::FromString("Stairs Length"));
		StairsLengthTitle->SetDesignBorderColor(FColor::Cyan);
	}


	if (StairsType_SB)
	{
		StairsType_SB->SetScrollBoxTitle(FText::FromString("Stairs Types"), FColor::Green);
	}
	return true;
}

void UExteriorWidget::HandleStairsTypeChanged(FText ElementText)
{
	for (FStairs_DA StairType : DataAsset->StairsTypes)
	{
		if (StairType.StairName.EqualTo(ElementText))
		{
			if (PlayerController)
			{
				PlayerController->ExteriorManager->ChangeStairsType(StairType.StairType);
				break;
			}
		}
	}

}

void UExteriorWidget::HandleDeleteButtonClicked()
{
	// Handle the delete button click event
	UE_LOG(LogTemp, Log, TEXT("Delete button clicked"));
	if (PlayerController)
	{
		PlayerController->ExteriorManager->DeleteStairs();
	}
}

void UExteriorWidget::HandleNumberOfStairsChanged(float Value)
{
	if (PlayerController)
	{
		PlayerController->ExteriorManager->ChangeNumberOfStairs(NumberOfStairsSpinBox->GetValue());
	}

}
void UExteriorWidget::HandleStairsDepthChanged(float Value)
{
	if (PlayerController)
	{
		PlayerController->ExteriorManager->ChangeStairsDepth(StairsDepthSpinBox->GetValue());
	}
}

void UExteriorWidget::HandleStairsLengthChanged(float Value)
{
	if (PlayerController)
	{
		PlayerController->ExteriorManager->ChangeStairsLength(StairsLengthSpinBox->GetValue());
	}
}

void UExteriorWidget::HandleStairsWidthChanged(float Value)
{
	if (PlayerController)
	{
		PlayerController->ExteriorManager->ChangeStairsWidth(StairsWidthSpinBox->GetValue());
	}
}


void UExteriorWidget::PopulateScrollBoxes()
{
	// Example data asset initialization
	if (DataAsset)
	{

		// Populate RoadState_SB
		for (const FStairs_DA& StairType : DataAsset->StairsTypes)
		{
			if (StairsType_SB)
			{
				StairsType_SB->AddScrollBoxElement(StairType.StairName, StairType.StairPreview);
			}
		}

	}

	// Bind element click events
	StairsType_SB->ElementSelected.BindUObject(this, &UExteriorWidget::HandleStairsTypeChanged);

}