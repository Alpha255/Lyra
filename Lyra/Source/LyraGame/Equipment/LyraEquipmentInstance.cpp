// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

ULyraEquipmentInstance::ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedActors);
}

APawn* ULyraEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnClass) const
{
    APawn* Pawn = nullptr;
    if (auto ActualPawnClass = PawnClass.Get())
    {
        if (GetOuter()->IsA(ActualPawnClass))
        {
            Pawn = Cast<APawn>(GetOuter());
        }
    }

    return Pawn;
}

void ULyraEquipmentInstance::OnEquipped()
{
    K2_OnEquipped();
}

void ULyraEquipmentInstance::OnUnequipped()
{
    K2_OnUnequipped();
}
