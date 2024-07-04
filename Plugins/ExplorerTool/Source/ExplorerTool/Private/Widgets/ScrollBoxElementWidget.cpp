
#include "Widgets/ScrollBoxElementWidget.h"

#include "Brushes/SlateColorBrush.h"
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
		ButtonName = NewElementText;
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

	ButtonNormal.SetNormal(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f)));
	ButtonNormal.SetHovered(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f)));
	ButtonNormal.SetPressed(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f)));

	ButtonPressed.SetNormal(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f)));
	ButtonPressed.SetHovered(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f)));
	ButtonPressed.SetPressed(FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f)));
}

void UScrollBoxElementWidget::HandleButtonClick()
{
	if (OnClickedDelegate.IsBound() && ElementName)
	{
		// Call the delegate and pass the text from ElementName
		OnClickedDelegate.Execute(ElementName->GetText());
	}
}
void UScrollBoxElementWidget::SetButtonSelected()
{
	ElementButton->SetStyle(ButtonPressed);
	bIsSelected = true;
}
void UScrollBoxElementWidget::SetButtonNormal()
{
	ElementButton->SetStyle(ButtonNormal);
	bIsSelected = false;
}
FText UScrollBoxElementWidget::GetButtonText()
{
	return ButtonName;
}