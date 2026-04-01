// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItem.h"
#include "InventoryFragment_ReticleConfig.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UInventoryFragment_ReticleConfig : public ULyraInventoryItemFragment
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reticle")
    TArray<TSubclassOf<class ULyraReticleWidgetBase>> ReticleWidgets;
};
