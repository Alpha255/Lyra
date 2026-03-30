// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraGameplayTags.h"

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
{
    if (InputTag.IsValid())
    {
        for (const auto& AbilitySpec : ActivatableAbilities.Items)
        {
            if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
            {
                InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
                InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
            }
        }
    }
}

void ULyraAbilitySystemComponent::AbilityInputTagReleased(FGameplayTag & InputTag)
{
    if (InputTag.IsValid())
    {
        for (const auto& AbilitySpec : ActivatableAbilities.Items)
        {
            if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
            {
                InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
                InputHeldSpecHandles.Remove(AbilitySpec.Handle);
            }
        }
    }
}

void ULyraAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
    if (HasMatchingGameplayTag(LyraGameplayTags::Ability_InputBlocked))
    {
        ClearAbilityInput();
        return;
    }

    TArray<FGameplayAbilitySpecHandle> AbilitiesToActive;

    for (const auto& SpecHandle : InputHeldSpecHandles)
    {
        if (const auto AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability && !AbilitySpec->IsActive())
            {
                const auto Ability = Cast<ULyraGameplayAbility>(AbilitySpec->Ability);
                if (Ability && Ability->GetActivationPolicy() == ELyraAbilityActivationPolicy::WhileInputActive)
                {
                    AbilitiesToActive.AddUnique(AbilitySpec->Handle);
                }
            }
        }
    }

    for (const auto& SpecHandle : InputPressedSpecHandles)
    {
        if (auto AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = true;
                
                if (AbilitySpec->IsActive())
                {
                    AbilitySpecInputPressed(*AbilitySpec);
                }
                else
                {
                    auto Ability = Cast<ULyraGameplayAbility>(AbilitySpec->Ability);
                    if (Ability && Ability->GetActivationPolicy() == ELyraAbilityActivationPolicy::OnInputTriggered)
                    {
                        AbilitiesToActive.AddUnique(AbilitySpec->Handle);
                    }
                }
            }
        }
    }

    for (const auto& SpecHandle : AbilitiesToActive)
    {
        TryActivateAbility(SpecHandle);
    }

    for (const auto& SpecHandle : InputHeldSpecHandles)
    {
        if (auto AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
        {
            if (AbilitySpec->Ability)
            {
                AbilitySpec->InputPressed = false;

                if (AbilitySpec->IsActive())
                {
                    AbilitySpecInputReleased(*AbilitySpec);
                }
            }
        }
    }

    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
}

void ULyraAbilitySystemComponent::ClearAbilityInput()
{
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}
