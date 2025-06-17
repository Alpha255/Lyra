// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraExperienceManagerComponent.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraExperienceLoaded, const class ULyraExperienceDefinition*);

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


UCLASS()
class LYRAGAME_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetExperience(FPrimaryAssetId AssetId);

private:
	UFUNCTION()
	void OnRep_Experience();

	void StartLoadExperience();
	void OnExperienceLoaded();
	void ApplyGameplayActions();

	ELyraExperienceLoadState ExperienceLoadState = ELyraExperienceLoadState::Unloaded;

	UPROPERTY(ReplicatedUsing=OnRep_Experience)
	TObjectPtr<const class ULyraExperienceDefinition> CurrentExperience;

	FOnLyraExperienceLoaded OnExperienceLoaded_HighPriority;
	FOnLyraExperienceLoaded OnExperienceLoaded_NormalPriority;
	FOnLyraExperienceLoaded OnExperienceLoaded_LowPriority;
};
