// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LyraInventoryItemInstance.h"
#include "Inventory/LyraInventoryItem.h"
#include "Net/UnrealNetwork.h"

void ULyraInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, InventoryItem);
}

const ULyraInventoryItemFragment* ULyraInventoryItemInstance::FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
    if (InventoryItem && FragmentClass)
    {
        return GetDefault<ULyraInventoryItem>(InventoryItem)->FindFragmentByClass(FragmentClass);
    }

    return nullptr;
}

void ULyraInventoryItemInstance::SetInventoryItem(TSubclassOf<ULyraInventoryItem> InInventoryItem)
{
    InventoryItem = InInventoryItem;
}
