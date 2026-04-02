// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LyraWeaponStateComponent.h"

ULyraWeaponStateComponent::ULyraWeaponStateComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

double ULyraWeaponStateComponent::GetTimeSinceLastHitNotification() const
{
    if (auto World = GetWorld())
    {
        return World->TimeSince(LastWeaponDamageInsitagatedTime);
    }

    return 0.0;
}
