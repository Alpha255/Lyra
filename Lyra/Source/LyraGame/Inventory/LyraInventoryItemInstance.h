// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraInventoryItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
    virtual bool IsSupportedForNetworking() const override { return true; }

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Meta = (DeterminesOutputType = FragmentClass))
    const class ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<class ULyraInventoryItemFragment> FragmentClass) const;

    template<class T>
    const T* FindFragmentByClass() const { return (T*)FindFragmentByClass(T::StaticClass()); }

    TSubclassOf<class ULyraInventoryItem> GetInventoryItem() const { return InventoryItem; }
private:
    friend struct FLyraInventoryList;

    void SetInventoryItem(TSubclassOf<class ULyraInventoryItem> InInventoryItem);

    UPROPERTY(Replicated)
    TSubclassOf<class ULyraInventoryItem> InventoryItem;
};
