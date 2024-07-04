// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ScrollBoxWidget.h"
#include "Widgets/ScrollBoxElementWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UScrollBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SB_Heading = CreateWidget<UTitleWidget>(this, TitleClass);
	SelectedButton = nullptr;
}


void UScrollBoxWidget::SetScrollBoxTitle(FText NewTitle, FLinearColor BorderColor)
{
	if (SB_Heading)
	{
		SB_Heading->SetTitleText(NewTitle);
		SB_Heading->SetDesignBorderColor(BorderColor);
	}
}

void UScrollBoxWidget::OnElementClicked(FText ElementText)
{
	UE_LOG(LogTemp, Log, TEXT("Element clicked: %s"), *ElementText.ToString());
	ElementSelected.ExecuteIfBound(ElementText);
	SetButtonSelected(ElementText);
}
void UScrollBoxWidget::ClearAllChildren()
{
	if (ElementScrollBox)
	{
		for (UScrollBoxElementWidget* Element : ScrollBoxButtons)
		{
			ElementScrollBox->RemoveChild(Element);
		}
	}
	ScrollBoxButtons.Empty();
}

void UScrollBoxWidget::AddScrollBoxElement(FText ElementName, UTexture2D* ElementIcon)
{
	if (ElementScrollBox)
	{
		UScrollBoxElementWidget* NewElement = CreateWidget<UScrollBoxElementWidget>(this, ScrollBoxElementClass);

		if (NewElement)
		{
			NewElement->SetElementNameAndIcon(ElementName, ElementIcon);
			
			// Bind the delegate to handle element click
			NewElement->OnClickedDelegate.BindUObject(this, &UScrollBoxWidget::OnElementClicked);

			ElementScrollBox->AddChild(NewElement);
			ScrollBoxButtons.Add(NewElement);
		}
	}
}
void UScrollBoxWidget::SetButtonSelected(FText ButtonText)
{
	if(ButtonText.IsEmptyOrWhitespace())
	{
		if(SelectedButton)
		{
			SelectedButton->SetButtonNormal();
			SelectedButton = nullptr;
		}
		
		return;
	}
	for(UScrollBoxElementWidget* ele: ScrollBoxButtons)
	{
		if(ele->GetButtonText().EqualTo(ButtonText))
		{
			if(SelectedButton == ele)
			{
				SelectedButton->SetButtonNormal();
				SelectedButton = nullptr;
				return;
			}
			ele->SetButtonSelected();
			if(SelectedButton)
			{
				SelectedButton->SetButtonNormal();
			}
			SelectedButton = ele;
			return;
		}
	}
}