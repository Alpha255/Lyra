// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraAbilitySystemComponent.h"

bool ULyraAbilitySystemComponent::IsActivationGroupBlocked(ELyraAbilityActivationGroup Group) const
{
    bool bBlocked = false;

    switch (Group)
    {
    case ELyraAbilityActivationGroup::Independent:
        return false;
    case ELyraAbilityActivationGroup::ExclusiveReplacealbe:
    case ELyraAbilityActivationGroup::ExclusiveBlocking:
        return NumActivationGroups[(uint8)ELyraAbilityActivationGroup::ExclusiveBlocking] > 0;
    }

    return false;
}

void ULyraAbilitySystemComponent::AbilityInputTagPressed(FGameplayTag& InputTag)
{}

void ULyraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag & InputTag)
{}

void ULyraAbilitySystemComponent::ProcessAbilityInputTag(float DeltaTime, bool bGamePaused)
{}

void ULyraAbilitySystemComponent::ClearAbilityInput()
{}
