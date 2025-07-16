// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Cosmetics/LyraCosmeticsAnimation.h"
#include "LyraWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraWeaponInstance : public ULyraEquipmentInstance
{
	GENERATED_BODY()
	
protected:
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Animation")
    TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const struct FGameplayTagContainer& CosmeticTags) const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    FLyraAnimLayerSelectionSet EquippedAnimSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    FLyraAnimLayerSelectionSet UnequippedAnimSet;
};
