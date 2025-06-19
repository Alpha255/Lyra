// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"

#include "LyraCosmeticsAnimation.generated.h"

USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Categories="Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(TitleProperty=Mesh))
	TArray<FLyraAnimBodyStyleSelectionEntry> MeshRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMesh> DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UPhysicsAsset> ForcedPhysicsAsset = nullptr;

	class USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& Tags) const;
};