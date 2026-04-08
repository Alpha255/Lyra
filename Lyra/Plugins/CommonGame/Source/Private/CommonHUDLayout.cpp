#include "CommonHUDLayout.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"

UCommonHUDLayout::UCommonHUDLayout(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UCommonActivatableWidgetContainerBase* UCommonHUDLayout::GetLayerWidget(FGameplayTag LayerTag)
{
    return Layers.FindRef(LayerTag);
}

void UCommonHUDLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
    if (!IsDesignTime())
    {
        LayerWidget->SetTransitionDuration(0.0f);
        Layers.Add(LayerTag, LayerWidget);
    }
}
