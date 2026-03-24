// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "LyraGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LYRAGAME_API ULyraGameplayAbility_Jump : public ULyraGameplayAbility
{
	GENERATED_BODY()
	
public:
    ULyraGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayTagContainer* SourceTags, 
        const FGameplayTagContainer* TargetTags, 
        FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
    void StartJump();

    UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
    void StopJump();
};
