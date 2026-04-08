#include "GameUIPolicy.h"
#include "CommonHUDLayout.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"

UCommonHUDLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
    auto LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
    return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

UWorld* UGameUIPolicy::GetWorld() const
{
    return GetOwningUIManager()->GetGameInstance()->GetWorld();
}

UGameUIManagerSubsystem* UGameUIPolicy::GetOwningUIManager() const
{
    return CastChecked<UGameUIManagerSubsystem>(GetOuter());
}

void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UCommonHUDLayout* Layout)
{
    UE_LOG(LogCore, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

    Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
    Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UCommonHUDLayout* Layout)
{
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
    if (auto PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
    {
        auto LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
        if (ensure(LayoutWidgetClass) && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))
        {
            auto LayoutWidget = CreateWidget<UCommonHUDLayout>(PlayerController, LayoutWidgetClass);
            RootViewportLayouts.Emplace(LocalPlayer, LayoutWidget, true);

            AddLayoutToViewport(LocalPlayer, LayoutWidget);
        }
    }
}

TSubclassOf<UCommonHUDLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
    return LayoutClass.LoadSynchronous();
}
