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

USTRUCT()
struct FLyraHUDLayoutRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "UI", meta = (AssetBundles="Client"))
    TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

    UPROPERTY(EditAnywhere, Category = "UI", meta = (Categories="UI Layer"))
    FGameplayTag LayerID;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Widgets"))
class UGameFeatureAction_AddWidget : public UGameFeatureAction_AddToWorld
{
	GENERATED_BODY()

public:
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

#if WITH_EDITORONLY_DATA
    virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif

#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

private:
    struct FPerActorData
    {
        TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
        //TArray<FUIExtensionHandle> ExtensionHandles;
    };

    struct FPerContextData
    {
        TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;
        TMap<FObjectKey, FPerActorData> ActorData;
    };

    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

    void Reset(FPerContextData& ActiveData);

    void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext& ChangeContext);

    void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
    void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);

    UPROPERTY(EditAnywhere, Category="UI", meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
    TArray<FLyraHUDLayoutRequest> Layout;

    UPROPERTY(EditAnywhere, Category="UI", meta = (TitleProperty = "{SlotID} -> {WidgetClass}"))
    TArray<FLyraHUDElementEntry> Widgets;
};
