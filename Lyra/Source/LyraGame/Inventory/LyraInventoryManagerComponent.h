// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "LyraInventoryManagerComponent.generated.h"


USTRUCT(BlueprintType)
struct FLyraInventoryEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<UActorComponent> Owner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TObjectPtr<class ULyraInventoryItemInstance> ItemInstance = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    int32 Num = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    int32 Delta = 0;
};

USTRUCT(BlueprintType)
struct FLyraInventoryList : public FFastArraySerializer
{
    GENERATED_BODY()

    FLyraInventoryList()
        : Owner(nullptr)
    {
    }

    FLyraInventoryList(UActorComponent* InOwner)
        : Owner(InOwner)
    {
    }

public:
    void PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FLyraInventoryEntry, FLyraInventoryList>(Entries, DeltaParms, *this);
    }

    class ULyraInventoryItemInstance* AddEntry(TSubclassOf<class ULyraInventoryItem> ItemClass, int32 Num);
    void AddEntry(class ULyraInventoryItemInstance* ItemInstance);
    void RemoveEntry(class ULyraInventoryItemInstance* ItemInstance);
private:
    friend class ULyraInventoryManagerComponent;

    void BroadcastInventoryChanged(FLyraInventoryEntry& Entry, int32 OldNum, int32 NewNum);

    UPROPERTY()
    TArray<FLyraInventoryEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UActorComponent> Owner;
};

template<>
struct TStructOpsTypeTraits<FLyraInventoryList> : public TStructOpsTypeTraitsBase2<FLyraInventoryList>
{
    enum
    {
        WithNetDeltaSerializer = true
    };
};

UCLASS(BlueprintType)
class LYRAGAME_API ULyraInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
    class ULyraInventoryItemInstance* AddInventoryItem(TSubclassOf<class ULyraInventoryItem> ItemClass, int32 Num);

private:
    UPROPERTY(Replicated)
    FLyraInventoryList InventoryList;
};
