#include "Widgets/MasterWidgetButton.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMasterWidgetButton::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind the OnClicked event to the ButtonClicked function
    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UMasterWidgetButton::ButtonClicked);
    }
}

void UMasterWidgetButton::SetButtonProperties(FText ButtonText, UTexture2D* ButtonIconParam, EArchVizMode Mode)
{
    if (ButtonTextBlock)
    {
        ButtonTextBlock->SetText(ButtonText);
    }

    if (ButtonIcon)
    {
        FSlateBrush Brush;
        Brush.SetResourceObject(ButtonIconParam);
        this->ButtonIcon->SetBrush(Brush);
    }
    ButtonMode = Mode;
}

void UMasterWidgetButton::ButtonClicked()
{
    ButtonSelected.ExecuteIfBound(ButtonMode);
}
