// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizUserWidget.h"
#include "ScrollBoxWidget.h"
#include "TitleWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "ConstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UConstructionWidget : public UArchVizUserWidget
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
	USizeBox* ContentSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ContentHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* ConstructionElements_SB;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* WallMaterial_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* RoofMaterial_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* FloorMaterial_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* SegmentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* SubSegmentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* DeleteContainerBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* DeleteTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* DeleteButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* DeleteIconContainer;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* DeleteIconSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* DeleteIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* DeleteDesignBorder;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScrollBoxWidget> ScrollBoxClass;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void HandleDeleteButtonClicked();
	void ClearSelected();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* DataAsset;
	virtual bool Initialize() override;

	UFUNCTION()
	void HandleConstructionElementChange(FText Text);
	UFUNCTION()
	void HandleWallMaterialChange(FText Text);
	void HandleFloorMaterialChange(FText Text);
	void HandleRoofMaterialChange(FText Text);
	UFUNCTION(BlueprintCallable)
	void PopulateScrollBoxes();
	void UpdatePropertiesSelected(UMaterialInterface* Material);
	void UpdatePropertiesSelected(UMaterialInterface* FloorMaterial, UMaterialInterface* RoofMaterial);
};

