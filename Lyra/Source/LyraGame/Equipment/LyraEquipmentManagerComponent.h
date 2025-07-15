// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "AbilitySystem/LyraGameplayAbilitySet.h"
#include "LyraEquipmentManagerComponent.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLyraEquipmentEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

private:
    friend class ULyraEquipmentManagerComponent;
    friend struct FLyraEquipmentList;

    UPROPERTY()
    TSubclassOf<class ULyraEquipment> EquipmentClass;

    UPROPERTY()
    TObjectPtr<class ULyraEquipmentInstance> EquipmentInstance;

    UPROPERTY(NotReplicated)
    FLyraGrantedGameplayAbilityHandles GrantedAbilityHandles;
};

USTRUCT(BlueprintType)
struct FLyraEquipmentList : public FFastArraySerializer
{
    GENERATED_BODY()

    FLyraEquipmentList()
        : Owner(nullptr)
    {
    }

    FLyraEquipmentList(UActorComponent* InOwner)
        : Owner(InOwner)
    {
    }

public:
    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FLyraEquipmentEntry, FLyraEquipmentList>(EquipmentEntries, DeltaParms, *this);
    }

    ULyraEquipmentInstance* AddEntry(TSubclassOf<ULyraEquipment> Equipment);
private:
    UPROPERTY()
    TArray<FLyraEquipmentEntry> EquipmentEntries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UActorComponent> Owner;
};

template<>
struct TStructOpsTypeTraits<FLyraEquipmentList> : public TStructOpsTypeTraitsBase2<FLyraEquipmentList>
{
    enum
    {
        WithNetDeltaSerializer = true
    };
};

UCLASS()
class LYRAGAME_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
    ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    class ULyraEquipmentInstance* EquipItem(TSubclassOf<class ULyraEquipment> EquipmentClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<class ULyraEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const;
private:
    UPROPERTY(Replicated)
    FLyraEquipmentList EquipmentList;
};
