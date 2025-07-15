// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "LyraGameplayAbilitySet.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FLyraGameplayAbilityConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULyraGameplayAbility> AbilityClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FLyraGrantedGameplayAbilityHandles
{
    GENERATED_BODY()

public:
    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
    {
        if (Handle.IsValid())
        {
            AbilitySpecHandles.Add(Handle);
        }
    }
protected:
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraGameplayAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    void GiveAbility(class ULyraAbilitySystemComponent* AbilitySysComp, FLyraGrantedGameplayAbilityHandles* OutGrantedAbilityHandles, UObject* Source = nullptr) const;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", Meta = (TitleProperty = AbilityClass))
	TArray<FLyraGameplayAbilityConfig> GrantedAbilities;
};
