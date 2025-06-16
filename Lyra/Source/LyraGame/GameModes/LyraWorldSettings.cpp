// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraWorldSettings.h"
#include "LyraGame/LyraLogChannel.h"
#include "System/LyraAssetManager.h"

ALyraWorldSettings::ALyraWorldSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FPrimaryAssetId ALyraWorldSettings::GetGameplayExperience() const
{
	FPrimaryAssetId AssetId;
	if (!GameplayExperience.IsNull())
	{
		AssetId = ULyraAssetManager::Get().GetPrimaryAssetIdForPath(GameplayExperience.ToSoftObjectPath());

		if (!AssetId.IsValid())
		{
			UE_LOG(LogLyra, Error, TEXT("Failed to resolve %s into an asset ID"), *GameplayExperience.ToString());
		}
	}

	return AssetId;
}
