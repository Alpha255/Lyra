// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraExperienceManagerComponent.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FLyraOnExperienceLoaded, const class ULyraExperienceDefinition*);

enum class ELyraExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

enum class ELyraExperienceLoadPriority
{
	Low,
	Normal,
	High
};


UCLASS()
class LYRAGAME_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetExperience(FPrimaryAssetId AssetId);

	void OnExperienceLoaded(FLyraOnExperienceLoaded::FDelegate&& Delegate, ELyraExperienceLoadPriority Priority);

	bool IsExperienceLoaded() { return ExperienceLoadState == ELyraExperienceLoadState::Loaded && CurrentExperience != nullptr; }

	const class ULyraExperienceDefinition* GetCurrentExperience() const;
private:
	UFUNCTION()
	void OnRep_Experience();

	void StartLoadExperience();
	void OnExperienceLoaded();
	void ApplyGameplayActions();

	ELyraExperienceLoadState ExperienceLoadState = ELyraExperienceLoadState::Unloaded;

	UPROPERTY(ReplicatedUsing=OnRep_Experience)
	TObjectPtr<const class ULyraExperienceDefinition> CurrentExperience;

	TArray<FLyraOnExperienceLoaded> OnExperienceLoadedDelegates;

	int32 NumGameFeaturePluginsToLoad = 0;
	TArray<FString> GameFeaturePluginURLs;
};
