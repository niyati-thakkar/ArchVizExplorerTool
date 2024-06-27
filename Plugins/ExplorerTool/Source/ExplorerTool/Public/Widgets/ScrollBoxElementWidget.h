#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/SizeBox.h"
#include "ArchVizUserWidget.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "ScrollBoxElementWidget.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(OnElementClicked, FText)

UCLASS()
class EXPLORERTOOL_API UScrollBoxElementWidget : public UArchVizUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ElementButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* OuterBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USizeBox* IconSizeBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ElementName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* BottomDesignBorder;

public:
	// Delegate to handle element clicked event
	OnElementClicked OnClickedDelegate;

	// Sets the text of the ElementName TextBlock
	UFUNCTION(BlueprintCallable)
	void SetElementNameAndIcon(FText NewElementText, UTexture2D* NewIcon);

protected:
	virtual void NativeConstruct() override;

private:
	// Handler for the button click event
	UFUNCTION()
	void HandleButtonClick();
};
