// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_Base.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UGameFeatureAction_Base : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

private:
    void OnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext Context);

    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& Context) PURE_VIRTUAL(UGameFeatureAction_Base::AddToWorld, );
private:
    TMap<FGameFeatureStateChangeContext, FDelegateHandle> ContextDelegateHandles;
};
