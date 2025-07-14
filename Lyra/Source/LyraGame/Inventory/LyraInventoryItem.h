// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItem.generated.h"

/**
 * 
 */

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class LYRAGAME_API ULyraInventoryItemFragment : public UObject
{
    GENERATED_BODY()

public:
    virtual void OnInstanceCreated(class ULyraInventoryItemInstance* Instance) const {}
};

UCLASS(Blueprintable, Const, Abstract)
class LYRAGAME_API ULyraInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
    const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
    FText DisplayName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display", Instanced)
    TArray<TObjectPtr<ULyraInventoryItemFragment>> Fragments;
};
