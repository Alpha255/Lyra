// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "UI/Weapons/SCircumferenceMarkerWidget.h"

#include "CircumferenceMarkerWidget.generated.h"

UCLASS()
class UCircumferenceMarkerWidget : public UWidget
{
    GENERATED_BODY()

public:
    UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer);

    virtual void SynchronizeProperties() override;

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetRadius(float NewRadius);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    TArray<FCircumferenceMarkerEntry> MarkerList;

    UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Appearance", meta = (ClampMin = 0.0))
    float Radius = 48.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    FSlateBrush MarkerBrush;

    UPROPERTY(EditAnywhere, Category = "Corner")
    uint8 bReticleCornerOutsizeSpreadRadius : 1;

private:
    TSharedPtr<SCircumferenceMarkerWidget> MarkerWidget;
};