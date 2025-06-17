// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LyraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static ULyraAssetManager& Get();

	const TMap<FSoftObjectPath, FPrimaryAssetId>& GetPrimaryAssetPathMap() const { return AssetPathMap; }
	const TMap<FSoftObjectPath, FSoftObjectPath>& GetAssetPathRedirects() const { return AssetPathRedirects; }
};
