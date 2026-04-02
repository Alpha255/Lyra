// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "GameplayTagContainer.h"
#include "HitMarkerConfirmationWidget.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UHitMarkerConfirmationWidget : public UWidget
{
	GENERATED_BODY()
	
public:
    UHitMarkerConfirmationWidget(const FObjectInitializer& ObjectInitializer);

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (ClampMin = 0.0, ForceUnits = s))
    float HitNotifyDuration = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    FSlateBrush PerHitMarkerBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    TMap<FGameplayTag, FSlateBrush> PerHitMarkerZoneOverBrushes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    FSlateBrush AnyHitMarkerBrush;

private:
    TSharedPtr<class SHitMarkerConfirmationWidget> MarkerWidget;
};
