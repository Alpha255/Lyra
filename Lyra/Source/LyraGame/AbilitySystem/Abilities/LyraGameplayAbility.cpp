// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/LyraCharacter.h"

ULyraGameplayAbility::ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
    
    ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
    ActivationGroup = ELyraAbilityActivationGroup::Independent;
}

AController* ULyraGameplayAbility::GetController() const
{
    if (CurrentActorInfo)
    {
        if (auto Controller = CurrentActorInfo->PlayerController.Get())
        {
            return Controller;
        }

        AActor* Owner = CurrentActorInfo->OwnerActor.Get();
        while (Owner)
        {
            if (auto Controller = Cast<AController>(Owner))
            {
                return Controller;
            }

            if (auto Pawn = Cast<APawn>(Owner))
            {
                return Pawn->GetController();
            }

            Owner = Owner->GetOwner();
        }
    }

    return nullptr;
}

ALyraCharacter* ULyraGameplayAbility::GetLyraCharacter() const
{
    return CurrentActorInfo ? Cast<ALyraCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

bool ULyraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayTagContainer * SourceTags, const FGameplayTagContainer * TargetTags, FGameplayTagContainer * OptionalRelevantTags) const
{
    return false;
}

void ULyraGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{}

void ULyraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{}

void ULyraGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{}

void ULyraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{}

void ULyraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{}

bool ULyraGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
    return false;
}

void ULyraGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{}

FGameplayEffectContextHandle ULyraGameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo) const
{
    return FGameplayEffectContextHandle();
}

void ULyraGameplayAbility::ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const
{}

bool ULyraGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent & AbilitySystemComponent, const FGameplayTagContainer * SourceTags, const FGameplayTagContainer * TargetTags, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
    return false;
}

void ULyraGameplayAbility::OnPawnSet()
{}

void ULyraGameplayAbility::GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, float& OutSourceLevel, const ILyraAbilitySourceInterface * &OutAbilitySource, AActor * &OutEffectCauser) const
{}
