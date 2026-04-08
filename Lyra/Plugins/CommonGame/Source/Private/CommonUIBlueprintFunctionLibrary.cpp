#include "CommonUIBlueprintFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "GameUIPolicy.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"
#include "CommonHUDLayout.h"

UCommonActivatableWidget* UCommonUIBlueprintFunctionLibrary::PushWidgetToLayerStack(const ULocalPlayer* LocalPlayer,
    FGameplayTag Layer,
    TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
    if (!ensure(LocalPlayer) || !ensure(WidgetClass))
    {
        return nullptr;
    }

    if (auto UIMgr = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
    {
        if (auto Policy = UIMgr->GetCurrentUIPolicy())
        {
            if (auto Layout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
            {
                return Layout->AddWidgetToLayerStack(Layer, WidgetClass);
            }
        }
    }

    return nullptr;
}