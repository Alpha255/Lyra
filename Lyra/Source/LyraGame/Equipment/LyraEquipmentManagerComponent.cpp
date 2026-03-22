// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/LyraAbilitySet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

void FLyraEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FLyraEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddIndices, int32 FinalSize)
{
}

void FLyraEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

ULyraEquipmentInstance* FLyraEquipmentList::AddEntry(TSubclassOf<ULyraEquipmentDefinition> Equipment)
{
    ULyraEquipmentInstance* EquipmentInstance = nullptr;
    check(Equipment);
    check(Owner);
    check(Owner->GetOwner()->HasAuthority());

    auto EquipmentCDO = GetDefault<ULyraEquipmentDefinition>(Equipment);
    auto InstanceClass = EquipmentCDO->EquipmentInstance ? EquipmentCDO->EquipmentInstance : TSubclassOf<ULyraEquipmentInstance>(ULyraEquipmentInstance::StaticClass());
    auto& NewEntry = EquipmentEntries.AddDefaulted_GetRef();
    NewEntry.EquipmentClass = Equipment;
    NewEntry.EquipmentInstance = NewObject<ULyraEquipmentInstance>(Owner->GetOwner(), InstanceClass);
    EquipmentInstance = NewEntry.EquipmentInstance;

    if (auto AbilitySysComp = GetAbilitySystemComponent())
    {
        for (auto& AbilitySet : EquipmentCDO->AbilitySets)
        {
            AbilitySet->GiveToAbilitySystem(AbilitySysComp, &NewEntry.GrantedAbilityHandles, EquipmentInstance);
        }
    }

    EquipmentInstance->SpawnEquipmentActor(EquipmentCDO->EquipmentsToSpawn);

    MarkItemDirty(NewEntry);

    return EquipmentInstance;
}

ULyraAbilitySystemComponent* FLyraEquipmentList::GetAbilitySystemComponent() const
{
    check(Owner);
    auto ParentActor = Owner->GetOwner();
    return Cast<ULyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ParentActor));
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

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentClass)
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
