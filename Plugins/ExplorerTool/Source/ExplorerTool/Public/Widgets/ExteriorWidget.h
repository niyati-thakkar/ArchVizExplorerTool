// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScrollBoxWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Widgets/ArchVizUserWidget.h"
#include "ExteriorWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORERTOOL_API UExteriorWidget : public UArchVizUserWidget
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
	UScrollBoxWidget* StairsType_SB;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* SegmentBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* NumberOfStairContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* NumberOfStairsTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpinBox* NumberOfStairsSpinBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* StairsWidthContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* StairsWidthTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpinBox* StairsWidthSpinBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* StairsLengthContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* StairsLengthTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpinBox* StairsLengthSpinBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* StairsDepthContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* StairsDepthTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USpinBox* StairsDepthSpinBox;

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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* SubSegmentBox;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTaskBarDataAsset* DataAsset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScrollBoxWidget> ScrollBoxClass;
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	UFUNCTION(BlueprintCallable)
	void HandleStairsWidthChanged(float Value);
	UFUNCTION(BlueprintCallable)
	void HandleStairsLengthChanged(float Value);
	UFUNCTION(BlueprintCallable)
	void HandleStairsDepthChanged(float Value);
	UFUNCTION(BlueprintCallable)
	void HandleNumberOfStairsChanged(float Value);
	UFUNCTION(BlueprintCallable)
	void HandleStairsTypeChanged(FText ElementText);
	void HandleDeleteButtonClicked();
	UFUNCTION(BlueprintCallable)
	void HandleDeleteButtonClicked(float RoadWidth);



	UFUNCTION(BlueprintCallable)
	void PopulateScrollBoxes();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoadState CurrentRoadState = ERoadState::StartRoad;
};
