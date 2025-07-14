// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItem.h"
#include "LyraInventoryFragment_EquippableItem.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraInventoryFragment_EquippableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Lyra")
    TSubclassOf<class ULyraEquipment> Equipment;
};
