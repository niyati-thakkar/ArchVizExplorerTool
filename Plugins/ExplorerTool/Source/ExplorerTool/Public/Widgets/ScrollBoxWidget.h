#pragma once

#include "CoreMinimal.h"
#include "TitleWidget.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "ArchVizUserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "ScrollBoxElementWidget.h"
#include "../TaskBarDataAsset.h"
#include "ScrollBoxWidget.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(OnElementSelected, FText)
UCLASS()
class EXPLORERTOOL_API UScrollBoxWidget : public UArchVizUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* PanelContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* SB_VerticalBox;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTitleWidget> TitleClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScrollBoxElementWidget> ScrollBoxElementClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTitleWidget* SB_Heading;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* ElementBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ElementScrollBox;

	// Sets the title text and border color for the TitleWidget
	UFUNCTION(BlueprintCallable)
	void SetScrollBoxTitle(FText NewTitle, FLinearColor BorderColor);

	UFUNCTION(BlueprintCallable)
	void AddScrollBoxElement(FText ElementName, UTexture2D* ElementIcon);
	void SetButtonSelected(FText ButtonText);
	void RemoveSelection();

	UFUNCTION(BlueprintCallable)
	void OnElementClicked(FText ElementText);
	void ClearAllChildren();

	OnElementSelected ElementSelected;

protected:
	virtual void NativeConstruct() override;
	UScrollBoxElementWidget* SelectedButton;
	TArray<UScrollBoxElementWidget*> ScrollBoxButtons;
};
