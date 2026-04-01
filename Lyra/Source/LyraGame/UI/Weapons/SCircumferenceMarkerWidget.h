// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Styling/CoreStyle.h"
#include "Widgets/Accessibility/SlateWidgetAccessibleTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SLeafWidget.h"

#include "SCircumferenceMarkerWidget.generated.h"

USTRUCT(BlueprintType)
struct FCircumferenceMarkerEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ForceUnits = deg))
    float PositionAngle = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ForceUnits = deg))
    float ImageRotationAngle = 1.0f;
};

class SCircumferenceMarkerWidget : public SLeafWidget
{

};