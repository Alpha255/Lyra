// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/LyraGameplayAbility.h"
#include "LyraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:
    bool IsActivationGroupBlocked(ELyraAbilityActivationGroup Group) const;

    void AbilityInputTagPressed(FGameplayTag& InputTag);
    void AbilityInputTagReleased(FGameplayTag& InputTag);

    void ProcessAbilityInputTag(float DeltaTime, bool bGamePaused);
    void ClearAbilityInput();
protected:
    int32 NumActivationGroups[(uint8)ELyraAbilityActivationGroup::MAX];

    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
