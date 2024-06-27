// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizUserWidget.h"
#include "ConstructionWidget.h"
#include "InteriorWidget.h"
#include "MasterWidgetButton.h"
#include "RoadWidget.h"
#include "SaveLoadWidget.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "ArchVizMasterWidget.generated.h"


/**
 *
 */
UCLASS()
class EXPLORERTOOL_API UArchVizMasterWidget : public UArchVizUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* TaskBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBackgroundBlur* TaskBarBackgroundBlur;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* TaskBarBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* TaskBarButtonBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMasterWidgetButton> MasterWidgetButtonClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMasterWidgetButton* RoadModeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMasterWidgetButton* ConstructionModeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMasterWidgetButton* InteriorModeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMasterWidgetButton* SaveLoadModeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LogoButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* LogoHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* LogoImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	URoadWidget* RoadModeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URoadWidget> RoadWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UConstructionWidget* ConstructionModeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UConstructionWidget> ConstructionWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UInteriorWidget* InteriorModeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInteriorWidget> InteriorWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USaveLoadWidget* SaveLoadModeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USaveLoadWidget> SaveLoadWidgetClass;
	AArchVizPlayerController* PlayerController;

	UFUNCTION()
	void ChangeToRoadMode();

	UFUNCTION()
	void ChangeToConstructionMode();

	UFUNCTION()
	void ChangeToInteriorMode();

	UFUNCTION()
	void ChangeToSaveLoadMode();

	void NativeConstruct() override;
	void RemoveButtonStyle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ButtonStyleTaskbar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ButtonStyleClicked;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* DataAsset;
};
