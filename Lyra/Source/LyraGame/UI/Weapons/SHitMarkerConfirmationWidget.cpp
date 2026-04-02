// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/SHitMarkerConfirmationWidget.h"
#include "Weapons/LyraWeaponStateComponent.h"

void SHitMarkerConfirmationWidget::Construct(const FArguments& InArgs, const FLocalPlayerContext& InContext, const TMap<FGameplayTag, FSlateBrush>& ZoneOverrideImages)
{
    PerHitMarkerBrush = InArgs._PerHitMarkerBrush;
    PerHitMarkerZoneOverrideBrushes = ZoneOverrideImages;
    AnyHitMarkerBrush = InArgs._AnyHitMarkerBrush;
    bSetColorAndOpacity = InArgs._ColorAndOpacity.IsSet();
    ColorAndOpacity = InArgs._ColorAndOpacity;
    LocalPlayerContext = InContext;
}

int32 SHitMarkerConfirmationWidget::OnPaint(const FPaintArgs& Args, 
    const FGeometry& AllottedGeometry, 
    const FSlateRect& MyCullingRect, 
    FSlateWindowElementList& OutDrawElements, 
    int32 LayerId, 
    const FWidgetStyle& InWidgetStyle, 
    bool bParentEnabled) const
{
    const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
    const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
    const FVector2D LocalCenter = AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.5f));
    const bool bDrawMarkers = HitNotifyOpacity > KINDA_SMALL_NUMBER;

    if (bDrawMarkers)
    {

    }

    return LayerId;
}

FVector2D SHitMarkerConfirmationWidget::ComputeDesiredSize(float) const
{
    return FVector2D(100.0f);
}

void SHitMarkerConfirmationWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    HitNotifyOpacity = 0.0f;

    if (auto PlayerController = LocalPlayerContext.IsInitialized() ? LocalPlayerContext.GetPlayerController() : nullptr)
    {
        if (auto WeaponStateComp = PlayerController->FindComponentByClass<ULyraWeaponStateComponent>())
        {
            double TimeSinceLastHitNotify = WeaponStateComp->GetTimeSinceLastHitNotification();
            if (TimeSinceLastHitNotify < HitNotifyDuration)
            {
                HitNotifyOpacity = FMath::Clamp(1.0f - (float)(TimeSinceLastHitNotify / HitNotifyDuration), 0.0f, 1.0f);
            }
        }
    }
}
