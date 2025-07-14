// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraGameplayAbilitySet.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraLogChannel.h"

void ULyraGameplayAbilitySet::GiveAbility(ULyraAbilitySystemComponent* AbilitySysComp, FLyraGrantedGameplayAbilityHandles* OutGrantedAbilityHandles, UObject* Source) const
{
    check(AbilitySysComp);

    if (!AbilitySysComp->IsOwnerActorAuthoritative())
    {
        return;
    }

    for (int32 Index = 0; Index < GrantedAbilities.Num(); ++Index)
    {
        const FLyraGameplayAbilityConfig& Config = GrantedAbilities[Index];

        if (!IsValid(Config.AbilityClass))
        {
            UE_LOG(LogLyra, Error, TEXT("Invalid AbilityClass in GrantedAbilities[%d] of %s"), Index, *GetNameSafe(this));
            continue;
        }

        auto AbilityCDO = Config.AbilityClass->GetDefaultObject<ULyraGameplayAbility>();
        FGameplayAbilitySpec AbilitySpec(AbilityCDO, Config.AbilityLevel, INDEX_NONE, Source);
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(Config.InputTag);

        const FGameplayAbilitySpecHandle GrantedHandle = AbilitySysComp->GiveAbility(AbilitySpec);
        if (OutGrantedAbilityHandles)
        {
            OutGrantedAbilityHandles->AddAbilitySpecHandle(GrantedHandle);
        }
    }
}
