// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/ScaleBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "ArchVizUserWidget.h"
#include "TitleWidget.generated.h"

/**
 *
 */
UCLASS()
class EXPLORERTOOL_API UTitleWidget : public UArchVizUserWidget
{
	GENERATED_BODY()
public:
	// Bindings to the UMG components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* OuterBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ContainerBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* DesignBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TitleBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScaleBox* DesignButtonScaleBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DesignButton;

	UFUNCTION(BlueprintCallable)
	void SetTitleText(FText NewTitleText);
	UFUNCTION(BlueprintCallable)
	void SetDesignBorderColor(FLinearColor NewBorderColor);
};
