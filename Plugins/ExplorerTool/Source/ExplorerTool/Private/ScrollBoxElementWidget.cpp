#include "ScrollBoxElementWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UScrollBoxElementWidget::SetElementNameAndIcon(FText NewElementText, UTexture2D* NewIcon)
{
	if (ElementName)
	{
		ElementName->SetText(NewElementText);
	}
	if (Icon && NewIcon)
	{
		IconSizeBox->SetHeightOverride(75);
		IconSizeBox->SetWidthOverride(75);
		Icon->SetBrushFromTexture(NewIcon);
	}
}

void UScrollBoxElementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the button click event to the handler function
	if (ElementButton)
	{
		ElementButton->OnClicked.AddDynamic(this, &UScrollBoxElementWidget::HandleButtonClick);
	}
}

void UScrollBoxElementWidget::HandleButtonClick()
{
	if (OnClickedDelegate.IsBound() && ElementName)
	{
		// Call the delegate and pass the text from ElementName
		OnClickedDelegate.Execute(ElementName->GetText());
	}
}
