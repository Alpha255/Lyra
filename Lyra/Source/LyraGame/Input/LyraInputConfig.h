// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "LyraInputConfig.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FLyraInputAction
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const class UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(Categories="InputTag"))
	FGameplayTag InputTag;
};


UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraInputConfig : public UDataAsset
{
	GENERATED_BODY()
	

public:
	ULyraInputConfig(const FObjectInitializer& ObjectInitializer);

    const class UInputAction* FindNativeInputActionForTag(const FGameplayTag& GameplayTag, bool bLogIfNotFound) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(TitleProperty="InputAction"))
	TArray<FLyraInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta=(TitleProperty="InputAction"))
	TArray<FLyraInputAction> AbilityInputActions;
};
