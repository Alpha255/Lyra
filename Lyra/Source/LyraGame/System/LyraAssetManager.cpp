// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LyraAssetManager.h"
#include "Character/LyraPawnData.h"

ULyraAssetManager& ULyraAssetManager::Get()
{
	check(GEngine);
	return *Cast<ULyraAssetManager>(&UAssetManager::Get());
}

UObject* ULyraAssetManager::LoadAssetSynchronous(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		auto LogTime = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Asset [%s] loaded"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);

		if (IsInitialized())
		{
			return GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		return AssetPath.TryLoad();
	}

	return nullptr;
}

const ULyraPawnData* ULyraAssetManager::GetDefaultPawnData() const
{
	return LoadAsset(DefaultPawnData);
}

void ULyraAssetManager::CacheAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Locker(&AssetCacheCS);
		AssetCache.Add(Asset);
	}
}

