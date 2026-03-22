// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpecHandle.h"
#include "AttributeSet.h"
#include "LyraAbilitySet.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GameplayAbility
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ULyraGameplayAbility> Ability = nullptr;

    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "InputTag"))
    FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GameplayEffect
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

    UPROPERTY(EditDefaultsOnly)
    float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_AttributeSet
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAttributeSet> AttributeSet;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GrantedHandles
{
    GENERATED_BODY()

public:
    void AddAttributeSet(UAttributeSet* Set)
    {
        if (Set)
        {
            GrantedAttributeSets.Add(Set);
        }
    }

    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
    {
        if (Handle.IsValid())
        {
            AbilitySpecHandles.Add(Handle);
        }
    }

    void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
    {
        if (Handle.IsValid())
        {
            GameplayEffectHandles.Add(Handle);
        }
    }
protected:
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

    UPROPERTY()
    TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

    UPROPERTY()
    TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};


UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    ULyraAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GiveToAbilitySystem(class ULyraAbilitySystemComponent* LyraASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty=Ability))
    TArray<FLyraAbilitySet_GameplayAbility> GrantedGameplayAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty=GameplayEffect))
    TArray<FLyraAbilitySet_GameplayEffect> GrantedGameplayEffects;

    UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty=AttributeSet))
    TArray<FLyraAbilitySet_AttributeSet> GrantedAttributeSets;
};
