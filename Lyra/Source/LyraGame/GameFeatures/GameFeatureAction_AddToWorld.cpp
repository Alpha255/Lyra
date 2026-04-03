// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddToWorld.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UGameFeatureAction_AddToWorld::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    GameInstanceStartDelegates.FindOrAdd(Context) = FWorldDelegates::OnStartGameInstance.AddUObject(this,
        &UGameFeatureAction_AddToWorld::OnGameInstanceStart, FGameFeatureStateChangeContext(Context));

    for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
    {
        if (Context.ShouldApplyToWorldContext(WorldContext))
        {
            AddToWorld(WorldContext, Context);
        }
    }
}

void UGameFeatureAction_AddToWorld::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    if (FDelegateHandle* Delegate = GameInstanceStartDelegates.Find(Context))
    {
        FWorldDelegates::OnStartGameInstance.Remove(*Delegate);
    }
}

void UGameFeatureAction_AddToWorld::OnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
{
    if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
    {
        if (ChangeContext.ShouldApplyToWorldContext(*WorldContext))
        {
            AddToWorld(*WorldContext, ChangeContext);
        }
    }
}
