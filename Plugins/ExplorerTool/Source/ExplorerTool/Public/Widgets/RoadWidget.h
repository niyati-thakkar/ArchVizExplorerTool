// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScrollBoxWidget.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/SizeBox.h"
#include "ArchVizUserWidget.h"
#include "RoadActors/ArchVizRoadActor.h"
#include "UMG/Public/Components/SpinBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "RoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API URoadWidget : public UArchVizUserWidget
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
	UScrollBoxWidget* RoadState_SB;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* RoadType_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBoxWidget* RoadMaterial_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* SegmentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* RoadWidthContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* RoadSizeTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoadWidthSpinBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* SubSegmentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* UndoContainerBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* UndoTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UndoButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* UndoIconContainer;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* UndoIconSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* UndoIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* UndoDesignBorder;
	
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* DataAsset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScrollBoxWidget> ScrollBoxClass;
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable)
	void HandleUndoButtonClicked();
	void UpdateMode(ERoadState roadState);
	UFUNCTION(BlueprintCallable)
	void HandleDeleteButtonClicked();
	UFUNCTION(BlueprintCallable)
	void HandleRoadTypeElement(FText ElementText);
	UFUNCTION(BlueprintCallable)
	void HandleRoadStateElement(FText ElementText);
	UFUNCTION(BlueprintCallable)
	void HandleRoadMaterialElement(FText ElementText);
	UFUNCTION(BlueprintCallable)
	void HandleWidthChange(float RoadWidth);
	
	
	
	UFUNCTION(BlueprintCallable)
	void PopulateScrollBoxes();
	UFUNCTION(BlueprintCallable)
	void UpdateWidth();
	void UpdatePropertiesSelected(UMaterialInterface* RoadMaterial, ERoadType Road, float RoadWidth);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ERoadState CurrentRoadState = ERoadState::StartRoad;
};
