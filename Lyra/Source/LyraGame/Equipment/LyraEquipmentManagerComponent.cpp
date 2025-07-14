// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Equipment/LyraEquipment.h"

ULyraEquipmentManagerComponent::ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SetIsReplicatedByDefault(true);
    bWantsInitializeComponent = true;
}

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraEquipment> EquipmentClass)
{
    ULyraEquipmentInstance* EquipmentInstance = nullptr;
    if (EquipmentClass)
    {
        if (EquipmentInstance)
        {
        }
    }
	return nullptr;
}

TArray<ULyraEquipmentInstance*> ULyraEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const
{
	return TArray<ULyraEquipmentInstance*>();
}
