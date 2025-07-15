// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipment.h"
#include "Equipment/LyraEquipmentInstance.h"

ULyraEquipment::ULyraEquipment(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstanceClass = ULyraEquipmentInstance::StaticClass();
}
