// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LocalPlayer.h"
#include "GameplayTagContainer.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateBrush.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Accessibility/SlateWidgetAccessibleTypes.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class SHitMarkerConfirmationWidget : public SLeafWidget
{
    SLATE_BEGIN_ARGS(SHitMarkerConfirmationWidget)
        : _PerHitMarkerBrush(FCoreStyle::Get().GetBrush("Throbber.CircleChunk"))
        , _AnyHitMarkerBrush(nullptr)
        , _HitNotifyDuration(0.4f)
        {
        }
        SLATE_ARGUMENT(const FSlateBrush*, PerHitMarkerBrush)
        SLATE_ARGUMENT(const FSlateBrush*, AnyHitMarkerBrush)
        SLATE_ATTRIBUTE(float, HitNotifyDuration)
        SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
    SLATE_END_ARGS()

public:
    void Construct(const FArguments& InArgs, const FLocalPlayerContext& InContext, const TMap<FGameplayTag, FSlateBrush>& ZoneOverrideImages);

    SHitMarkerConfirmationWidget() {}

    // SWidget interface
    virtual int32 OnPaint(const FPaintArgs& Args, 
        const FGeometry& AllottedGeometry, 
        const FSlateRect& MyCullingRect, 
        FSlateWindowElementList& OutDrawElements, 
        int32 LayerId, 
        const FWidgetStyle& InWidgetStyle, 
        bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override;

    virtual bool ComputeVolatility() const override { return true; }

    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
    const FSlateBrush* PerHitMarkerBrush = nullptr;
    const FSlateBrush* AnyHitMarkerBrush = nullptr;
    float HitNotifyOpacity = 0.0f;
    float HitNotifyDuration = 0.4f;

    TMap<FGameplayTag, FSlateBrush> PerHitMarkerZoneOverrideBrushes;
    
    TAttribute<FSlateColor> ColorAndOpacity;
    bool bSetColorAndOpacity;

    FLocalPlayerContext LocalPlayerContext;
};
