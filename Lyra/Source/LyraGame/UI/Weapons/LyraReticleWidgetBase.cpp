// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/LyraReticleWidgetBase.h"
#include "Weapons/LyraWeaponInstance.h"
#include "Weapons/LyraRangedWeaponInstance.h"
#include "Inventory/LyraInventoryItemInstance.h"

ULyraReticleWidgetBase::ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULyraReticleWidgetBase::InitializeFromWeapon(ULyraWeaponInstance* Weapon)
{
    WeaponInstance = Weapon;
    InventoryItemInstance = nullptr;

    if (WeaponInstance)
    {
        InventoryItemInstance = Cast<ULyraInventoryItemInstance>(WeaponInstance->GetInstigator());
    }

    OnWeaponInitialize();
}

float ULyraReticleWidgetBase::ComputeSpreadAngle() const
{
    return 0.0f;
}

float ULyraReticleWidgetBase::ComputeMaxSpreadRadiusScreenspace() const
{
    return 0.0f;
}

bool ULyraReticleWidgetBase::HasFirstShotAccuracy() const
{
    return false;
}
