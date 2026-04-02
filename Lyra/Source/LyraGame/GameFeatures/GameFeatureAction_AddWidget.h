// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "GameFeatures/GameFeatureAction_AddToWorld.h"
#include "GameFeatureAction_AddWidget.generated.h"

/**
 * 
 */

USTRUCT()
struct FLyraHUDElementEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="UI", meta = (AssetBundles = "Client"))
    TSoftClassPtr<UUserWidget> WidgetClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    FGameplayTag SlotID;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Widgets"))
class UGameFeatureAction_AddWidget : public UGameFeatureAction_AddToWorld
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category="UI", meta = (TitleProperty = "{SlotID} -> {WidgetClass}"))
    TArray<FLyraHUDElementEntry> Widgets;
};
