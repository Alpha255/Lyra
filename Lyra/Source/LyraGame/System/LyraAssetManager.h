// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LyraAssetManager.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class LYRAGAME_API ULyraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static ULyraAssetManager& Get();

	static UObject* LoadAssetSynchronous(const FSoftObjectPath& AssetPath);

	template<class T>
	static T* LoadAsset(const TSoftObjectPtr<T>& SoftAssetPtr, bool bCache = false)
	{
		T* Asset = nullptr;
		auto AssetPath = SoftAssetPtr.ToSoftObjectPath();
		if (AssetPath.IsValid())
		{
			Asset = SoftAssetPtr.Get();
			if (!Asset)
			{
				Asset = Cast<T>(LoadAssetSynchronous(AssetPath));
				ensureAlwaysMsgf(Asset, TEXT("Failed to load asset [%s]"), *SoftAssetPtr.ToString());
			}

			if (Asset && bCache)
			{
				Get().CacheAsset(Cast<UObject>(Asset));
			}
		}

		return Asset;
	}

	const TMap<FSoftObjectPath, FPrimaryAssetId>& GetPrimaryAssetPathMap() const { return AssetPathMap; }
	const TMap<FSoftObjectPath, FSoftObjectPath>& GetAssetPathRedirects() const { return AssetPathRedirects; }

	const class ULyraPawnData* GetDefaultPawnData() const;
protected:
	void CacheAsset(const UObject* Asset);

	UPROPERTY(Config)
	TSoftObjectPtr<class ULyraPawnData> DefaultPawnData;
private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> AssetCache;

	FCriticalSection AssetCacheCS;
};
