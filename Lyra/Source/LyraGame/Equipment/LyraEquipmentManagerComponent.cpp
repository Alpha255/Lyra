// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/LyraEquipmentInstance.h"

TArray<ULyraEquipmentInstance*> ULyraEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const
{
	return TArray<ULyraEquipmentInstance*>();
}
