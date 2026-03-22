// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LyraInventoryItem.h"

ULyraInventoryItem::ULyraInventoryItem(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

const ULyraInventoryItemFragment* ULyraInventoryItem::FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
    if (FragmentClass)
    {
        for (auto Fragment : Fragments)
        {
            if (Fragment && Fragment->IsA(FragmentClass))
            {
                return Fragment;
            }
        }
    }

    return nullptr;
}
