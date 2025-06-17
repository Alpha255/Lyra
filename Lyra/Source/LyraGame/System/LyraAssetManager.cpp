// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LyraAssetManager.h"

ULyraAssetManager& ULyraAssetManager::Get()
{
	check(GEngine);

	return *Cast<ULyraAssetManager>(&UAssetManager::Get());
}

