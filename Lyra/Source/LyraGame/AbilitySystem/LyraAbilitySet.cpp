// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraAbilitySet.h"
#include "LyraAbilitySet.h"
#include "LyraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

ULyraAbilitySet::ULyraAbilitySet(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULyraAbilitySet::GiveToAbilitySystem(ULyraAbilitySystemComponent* LyraASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
    check(LyraASC);

    if (!LyraASC->IsOwnerActorAuthoritative())
    {
        return;
    }

    for (int32 Index = 0; Index < GrantedAttributeSets.Num(); ++Index)
    {
        auto& GrantedAttributeSet = GrantedAttributeSets[Index];

        if (!IsValid(GrantedAttributeSet.AttributeSet))
        {
            UE_LOG(LogAbilitySystemComponent, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), Index, *GetNameSafe(this));
            continue;
        }

        UAttributeSet* Set = NewObject<UAttributeSet>(LyraASC->GetOwner(), GrantedAttributeSet.AttributeSet);
        LyraASC->AddAttributeSetSubobject(Set);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAttributeSet(Set);
        }
    }

    for (int32 Index = 0; Index < GrantedGameplayAbilities.Num(); ++Index)
    {
        auto& GrantedAbility = GrantedGameplayAbilities[Index];

        if (!IsValid(GrantedAbility.Ability))
        {
            UE_LOG(LogAbilitySystemComponent, Error, TEXT("GrantedGameplayAbilities [%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
            continue;
        }

        auto DefaultAbility = GrantedAbility.Ability->GetDefaultObject<ULyraGameplayAbility>();
        FGameplayAbilitySpec AbilitySpec(DefaultAbility, GrantedAbility.AbilityLevel);
        AbilitySpec.SourceObject = SourceObject;
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(GrantedAbility.InputTag);

        auto AbilitySpecHandle = LyraASC->GiveAbility(AbilitySpec);

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
        }
    }

    for (int32 Index = 0; Index < GrantedGameplayEffects.Num(); ++Index)
    {
        auto& GrantedEffect = GrantedGameplayEffects[Index];

        if (!IsValid(GrantedEffect.GameplayEffect))
        {
            UE_LOG(LogAbilitySystemComponent, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
            continue;
        }

        auto DefaultGameplayEffect = GrantedEffect.GameplayEffect->GetDefaultObject<UGameplayEffect>();
        auto GameplayEffectHandle = LyraASC->ApplyGameplayEffectToSelf(DefaultGameplayEffect, GrantedEffect.EffectLevel, LyraASC->MakeEffectContext());

        if (OutGrantedHandles)
        {
            OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
        }
    }
}
