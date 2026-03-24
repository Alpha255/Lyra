// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility_Jump.h"
#include "LyraGameplayAbility_Jump.h"
#include "Character/LyraCharacter.h"

ULyraGameplayAbility_Jump::ULyraGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool ULyraGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayTagContainer* SourceTags, 
    const FGameplayTagContainer* TargetTags, 
    FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        return false;
    }

    auto Character = Cast<ALyraCharacter>(ActorInfo->AvatarActor.Get());
    if (!Character || !Character->CanJump())
    {
        return false;
    }

    return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULyraGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    bool bReplicateEndAbility, 
    bool bWasCancelled)
{
    StopJump();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULyraGameplayAbility_Jump::StartJump()
{
    if (auto Character = GetLyraCharacter())
    {
        if (Character->IsLocallyControlled() && !Character->bPressedJump)
        {
            Character->UnCrouch();
            Character->Jump();
        }
    }
}

void ULyraGameplayAbility_Jump::StopJump()
{
    if (auto Character = GetLyraCharacter())
    {
        if (Character->IsLocallyControlled() && Character->bPressedJump)
        {
            Character->StopJumping();
        }
    }
}
