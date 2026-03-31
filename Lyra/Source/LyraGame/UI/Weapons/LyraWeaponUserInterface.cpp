// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Weapons/LyraWeaponUserInterface.h"
#include "GameFramework/Pawn.h"
#include "Weapons/LyraWeaponInstance.h"
#include "Equipment/LyraEquipmentManagerComponent.h"

ULyraWeaponUserInterface::ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULyraWeaponUserInterface::NativeConstruct()
{
    Super::NativeConstruct();
}

void ULyraWeaponUserInterface::NativeDestruct()
{
    Super::NativeDestruct();
}

void ULyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (auto Pawn = GetOwningPlayerPawn())
    {
        if (auto EquipmentMgr = Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>())
        {
            if (auto FirstInstance = EquipmentMgr->GetFirstInstanceOfType<ULyraWeaponInstance>())
            {
                if (WeaponInstance != FirstInstance && FirstInstance->GetInstigator())
                {
                    auto OldWeapon = WeaponInstance;
                    WeaponInstance = FirstInstance;
                    RebuildWidgetFromWeapon();
                    OnWeaponChanged(OldWeapon, WeaponInstance);
                }
            }
        }
    }
}

void ULyraWeaponUserInterface::RebuildWidgetFromWeapon()
{}
