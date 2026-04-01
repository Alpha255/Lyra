#include "CircumferenceMarkerWidget.h"

UCircumferenceMarkerWidget::UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SetVisibility(ESlateVisibility::HitTestInvisible);
    bIsVolatile = true;
}

void UCircumferenceMarkerWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    MarkerWidget->SetRadius(Radius);
    MarkerWidget->SetMarkerList(MarkerList);
}

void UCircumferenceMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MarkerWidget.Reset();
}

TSharedRef<SWidget> UCircumferenceMarkerWidget::RebuildWidget()
{
    MarkerWidget = SNew(SCircumferenceMarkerWidget)
        .MarkerBrush(&this->MarkerBrush)
        .Radius(this->Radius)
        .MarkerList(this->MarkerList);

    return MarkerWidget.ToSharedRef();
}

void UCircumferenceMarkerWidget::SetRadius(float NewRadius)
{
    Radius = NewRadius;

    if (MarkerWidget.IsValid())
    {
        MarkerWidget->SetRadius(NewRadius);
    }
}
