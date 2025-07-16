// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LyraInventoryManagerComponent.h"
#include "Inventory/LyraInventoryItem.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

void FLyraInventoryList::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
}

void FLyraInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FLyraInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

ULyraInventoryItemInstance* FLyraInventoryList::AddEntry(TSubclassOf<ULyraInventoryItem> ItemClass, int32 Num)
{
    ULyraInventoryItemInstance* Instance = nullptr;

    check(ItemClass && Owner);
    check(Owner->GetOwner()->HasAuthority());

    auto& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.ItemInstance = NewObject<ULyraInventoryItemInstance>(Owner->GetOwner());
    NewEntry.ItemInstance->SetInventoryItem(ItemClass);
    NewEntry.Num = Num;

    Instance = NewEntry.ItemInstance;
    MarkItemDirty(NewEntry);

    return Instance;
}

void FLyraInventoryList::AddEntry(ULyraInventoryItemInstance* ItemInstance)
{
}

void FLyraInventoryList::RemoveEntry(ULyraInventoryItemInstance* ItemInstance)
{
}

void FLyraInventoryList::BroadcastInventoryChanged(FLyraInventoryEntry& Entry, int32 OldNum, int32 NewNum)
{
}

ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , InventoryList(this)
{
    SetIsReplicatedByDefault(true);
}

void ULyraInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULyraInventoryManagerComponent, InventoryList);
}

ULyraInventoryItemInstance* ULyraInventoryManagerComponent::AddInventoryItem(TSubclassOf<ULyraInventoryItem> ItemClass, int32 Num)
{
    ULyraInventoryItemInstance* Instance = nullptr;
    if (ItemClass)
    {
        Instance = InventoryList.AddEntry(ItemClass, Num);

        if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Instance)
        {
            AddReplicatedSubObject(Instance);
        }
    }

    return Instance;
}
