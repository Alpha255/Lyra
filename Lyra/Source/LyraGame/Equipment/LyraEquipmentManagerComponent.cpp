// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Equipment/LyraEquipment.h"
#include "Net/UnrealNetwork.h"

void FLyraEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FLyraEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddIndices, int32 FinalSize)
{
}

void FLyraEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

ULyraEquipmentInstance* FLyraEquipmentList::AddEntry(TSubclassOf<ULyraEquipment> Equipment)
{
    ULyraEquipmentInstance* EquipmentInstance = nullptr;
    check(Equipment);
    check(Owner);
    check(Owner->GetOwner()->HasAuthority());

    auto EquipmentCDO = GetDefault<ULyraEquipment>(Equipment);
    auto InstanceClass = EquipmentCDO->InstanceClass ? EquipmentCDO->InstanceClass : TSubclassOf<ULyraEquipmentInstance>(ULyraEquipmentInstance::StaticClass());
    auto& NewEntry = EquipmentEntries.AddDefaulted_GetRef();
    NewEntry.EquipmentClass = Equipment;
    NewEntry.EquipmentInstance = NewObject<ULyraEquipmentInstance>(Owner->GetOwner(), InstanceClass);
    EquipmentInstance = NewEntry.EquipmentInstance;

    MarkItemDirty(NewEntry);

    return EquipmentInstance;
}

ULyraEquipmentManagerComponent::ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , EquipmentList(this)
{
    SetIsReplicatedByDefault(true);
    bWantsInitializeComponent = true;
}

void ULyraEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, EquipmentList);
}

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraEquipment> EquipmentClass)
{
    ULyraEquipmentInstance* EquipmentInstance = nullptr;
    if (EquipmentClass)
    {
        EquipmentInstance = EquipmentList.AddEntry(EquipmentClass);
        if (EquipmentInstance)
        {
            EquipmentInstance->OnEquipped();

            if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
            {
                AddReplicatedSubObject(EquipmentInstance);
            }
        }
    }

	return EquipmentInstance;
}

TArray<ULyraEquipmentInstance*> ULyraEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const
{
	return TArray<ULyraEquipmentInstance*>();
}
