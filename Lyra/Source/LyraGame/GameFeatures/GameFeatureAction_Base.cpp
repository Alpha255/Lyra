// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_Base.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UGameFeatureAction_Base::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    // Register the game instance start delegate
    ContextDelegateHandles.FindOrAdd(Context) = FWorldDelegates::OnStartGameInstance.AddUObject(
        this, 
        &UGameFeatureAction_Base::OnGameInstanceStart, 
        FGameFeatureStateChangeContext(Context));

    // Add to any worlds with associated game instances that have already been initialized
    for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
    {
        if (Context.ShouldApplyToWorldContext(WorldContext))
        {
            AddToWorld(WorldContext, Context);
        }
    }
}

void UGameFeatureAction_Base::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    // Unregister the game instance start delegate
    if (FDelegateHandle* Handle = ContextDelegateHandles.Find(Context))
    {
        FWorldDelegates::OnStartGameInstance.Remove(*Handle);
    }
    // Cleanup any resources or state if necessary
    // This is where you would typically remove components or clean up any state related to the feature
}

void UGameFeatureAction_Base::OnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext Context)
{
    if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
    {
        if (Context.ShouldApplyToWorldContext(*WorldContext))
        {
            AddToWorld(*WorldContext, Context);
        }
    }
}
