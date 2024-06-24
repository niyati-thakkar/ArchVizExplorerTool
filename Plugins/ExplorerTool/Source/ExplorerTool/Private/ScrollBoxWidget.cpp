// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollBoxWidget.h"
#include "ScrollBoxElementWidget.h"
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
		}
	}
}
