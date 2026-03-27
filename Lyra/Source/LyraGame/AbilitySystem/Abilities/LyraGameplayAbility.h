// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LyraGameplayAbility.generated.h"


UENUM(BlueprintType)
enum class ELyraAbilityActivationPolicy : uint8
{
    OnInputTriggered,
    WhileInputActive,
    OnSpawn
};

UENUM(BlueprintType)
enum class ELyraAbilityActivationGroup : uint8
{
    Independent,
    ExclusiveReplacealbe,
    ExclusiveBlocking,
    MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input)
class LYRAGAME_API ULyraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    ULyraGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        const FGameplayTagContainer* SourceTags, 
        const FGameplayTagContainer* TargetTags, 
        FGameplayTagContainer* OptionalRelevantTags) const override;

    virtual void SetCanBeCanceled(bool bCanBeCanceled) override;

    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled) override;

    virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

    virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;

    virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;

    virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
        const FGameplayTagContainer* SourceTags = nullptr,
        const FGameplayTagContainer* TargetTags = nullptr,
        OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

    virtual void OnPawnSet();

    virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, 
        const FGameplayAbilityActorInfo* ActorInfo, 
        float& OutSourceLevel, 
        const class ILyraAbilitySourceInterface*& OutAbilitySource, 
        AActor*& OutEffectCauser) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnAbilityAdded")
    void K2_OnAbilityAdded();

    UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnAbilityRemoved")
    void K2_OnAbilityRemoved();

    UFUNCTION(BlueprintImplementableEvent, Category = "Ability", DisplayName = "OnPawnSet")
    void K2_OnPawnSet();

public:
    UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
    AController* GetController() const;

    UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
    class ALyraCharacter* GetLyraCharacter() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
    ELyraAbilityActivationPolicy ActivationPolicy;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
    ELyraAbilityActivationGroup ActivationGroup;

    UPROPERTY(EditDefaultsOnly, Category = "Debug")
    bool bLogOnCancel = false;

    TSubclassOf<class ULyraCameraMode> ActiveCameraMode = nullptr;
};
