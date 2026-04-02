// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_AddToWorld.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LYRAGAME_API UGameFeatureAction_AddToWorld : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

private:
    void OnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);

    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_AddToWorld::AddToWorld,);

private:
    TMap<FGameFeatureStateChangeContext, FDelegateHandle> GameInstanceStartDelegates;
};
