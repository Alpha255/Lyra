// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LyraWeaponInstance.h"

TSubclassOf<UAnimInstance> ULyraWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
    return bEquipped ? EquippedAnimSet.SelectBestLayer(CosmeticTags) : UnequippedAnimSet.SelectBestLayer(CosmeticTags);
}
