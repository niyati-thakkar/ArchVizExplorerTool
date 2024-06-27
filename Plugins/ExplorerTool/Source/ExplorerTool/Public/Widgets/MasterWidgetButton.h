// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Game/ArchVizPlayerController.h"

#include "MasterWidgetbutton.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(OnButtonSelected, EArchVizMode)
UCLASS()
class EXPLORERTOOL_API UMasterWidgetButton : public UArchVizUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SeparatorImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ButtonIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ButtonTextBlock;

	UPROPERTY(BlueprintReadWrite)
	EArchVizMode ButtonMode;

	void NativeConstruct();
	void SetButtonProperties(FText ButtonText, UTexture2D* ButtonIcon, EArchVizMode Mode);

	OnButtonSelected ButtonSelected;

	void ButtonClicked();
};
