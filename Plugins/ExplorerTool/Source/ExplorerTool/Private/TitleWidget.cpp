// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"

void UTitleWidget::SetTitleText(FText NewTitleText)
{
	if (TitleBox)
	{
		TitleBox->SetText(NewTitleText);
	}
}

void UTitleWidget::SetDesignBorderColor(FLinearColor NewBorderColor)
{
	if (DesignBorder)
	{
		DesignBorder->SetBrushColor(NewBorderColor);
	}
}
