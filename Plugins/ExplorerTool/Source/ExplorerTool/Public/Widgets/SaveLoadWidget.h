// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizUserWidget.h"
#include "ScrollBoxWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "SaveLoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API USaveLoadWidget : public UArchVizUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ParentVerticalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PageTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SpacerImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ContentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ContentHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* ExistingProjects_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ButtonBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxElementWidget* LoadButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxElementWidget* SaveButton;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* NewButtonBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxElementWidget* NewButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* NewItemTextBox;
	void HandleSaveButtonClicked(FText Text);

	void PopulateScrollBoxes(); UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* DataAsset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScrollBoxElementWidget> ScrollBoxElementClass;

	virtual void NativeConstruct() override;
	void HandleLoadButtonClicked(FText Text);
	void HandleNewButtonClicked(FText Text);
	virtual bool Initialize() override;
	void UpdateCurrentSlot(FText SlotName);
};

