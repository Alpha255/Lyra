// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/HitMarkerConfirmationWidget.h"
#include "UI/Weapons/SHitMarkerConfirmationWidget.h"
#include "Blueprint/UserWidget.h"

UHitMarkerConfirmationWidget::UHitMarkerConfirmationWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SetVisibility(ESlateVisibility::HitTestInvisible);
    bIsVolatile = true;
    AnyHitMarkerBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
}

void UHitMarkerConfirmationWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MarkerWidget.Reset();
}

TSharedRef<SWidget> UHitMarkerConfirmationWidget::RebuildWidget()
{
    auto OuterWidget = GetTypedOuter<UUserWidget>();
    FLocalPlayerContext LocalPlayerContext;
    const FLocalPlayerContext& ContextRef = OuterWidget ? OuterWidget->GetPlayerContext() : LocalPlayerContext;

    MarkerWidget = SNew(SHitMarkerConfirmationWidget, ContextRef, PerHitMarkerZoneOverBrushes)
        .PerHitMarkerBrush(&this->PerHitMarkerBrush)
        .AnyHitMarkerBrush(&this->AnyHitMarkerBrush)
        .HitNotifyDuration(this->HitNotifyDuration);

    return MarkerWidget.ToSharedRef();
}
