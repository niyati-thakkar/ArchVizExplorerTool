#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "ScrollBoxElementWidget.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(OnElementClicked, FText)

UCLASS()
class EXPLORERTOOL_API UScrollBoxElementWidget : public UUserWidget
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
