// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/LyraCharacter.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraGameplayTags.h"
#include "Camera/LyraCameraMode.h"
#include "Character/LyraHeroComponent.h"

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

ULyraHeroComponent* ULyraGameplayAbility::GetHeroComponent()
{
    return CurrentActorInfo ? ULyraHeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

void ULyraGameplayAbility::ClearCameraMode()
{
    if (ActiveCameraMode)
    {
        if (auto HeroComponent = GetHeroComponent())
        {
            HeroComponent->ClearAbilityCameraMode(CurrentSpecHandle);
        }

        ActiveCameraMode = nullptr;
    }
}

void ULyraGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
    if (ActorInfo && !Spec.IsActive() && ActivationPolicy == ELyraAbilityActivationPolicy::OnSpawn)
    {
        auto AbilitySysComp = ActorInfo->AbilitySystemComponent.Get();
        const AActor* Actor = ActorInfo->AvatarActor.Get();

        if (AbilitySysComp && Actor && !Actor->GetTearOff() && Actor->GetLifeSpan() <= 0.0f)
        {
            const bool bLocalExec = NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted || NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly;
            const bool bServerExec = NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly || NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated;

            const bool bShouldActivateClient = bLocalExec && ActorInfo->IsLocallyControlled();
            const bool bShouldActivateServer = bServerExec && ActorInfo->IsNetAuthority();

            if (bShouldActivateClient || bShouldActivateServer)
            {
                AbilitySysComp->TryActivateAbility(Spec.Handle);
            }
        }
    }
}

bool ULyraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
    {
        return false;
    }

    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    auto LyraASC = CastChecked<ULyraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
    if (LyraASC->IsActivationGroupBlocked(ActivationGroup))
    {
        if (OptionalRelevantTags)
        {
            OptionalRelevantTags->AddTag(LyraGameplayTags::Ability_ActivateFail_ActivationGroup);
        }

        return false;
    }

    return true;
}

void ULyraGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
    if (!bCanBeCanceled && ActivationGroup == ELyraAbilityActivationGroup::ExclusiveReplacealbe)
    {
        return;
    }

    Super::SetCanBeCanceled(bCanBeCanceled);
}

void ULyraGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    K2_OnAbilityAdded();

    TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void ULyraGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
    K2_OnAbilityRemoved();

    Super::OnRemoveAbility(ActorInfo, Spec);
}

void ULyraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void ULyraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    ClearCameraMode();

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool ULyraGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
    return false;
}

void ULyraGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
}

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
