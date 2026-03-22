// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipmentInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "LyraEquipmentDefinition.h"

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

APawn* ULyraEquipmentInstance::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

void ULyraEquipmentInstance::SpawnEquipmentActor(const TArray<FLyraEquipmentToSpawn>& ActorsToSpawn)
{
    if (auto Pawn = GetPawn())
    {
        auto AttachTo = Pawn->GetRootComponent();
        if (auto Character = Cast<ACharacter>(Pawn))
        {
            AttachTo = Character->GetMesh();
        }
        
        for (auto& ActorToSpawn : ActorsToSpawn)
        {
            auto Actor = GetWorld()->SpawnActorDeferred<AActor>(ActorToSpawn.ActorToSpawn, FTransform::Identity, Pawn);
            Actor->FinishSpawning(FTransform::Identity, true);
            Actor->SetActorRelativeTransform(ActorToSpawn.AttachTransform);
            Actor->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepRelativeTransform, ActorToSpawn.AttachSocket);

            SpawnedActors.Add(Actor);
        }
    }
}

void ULyraEquipmentInstance::DestroyEquipmentActors()
{
    for (auto Actor : SpawnedActors)
    {
        if (Actor)
        {
            Actor->Destroy();
        }
    }
}

void ULyraEquipmentInstance::OnEquipped()
{
    K2_OnEquipped();
}

void ULyraEquipmentInstance::OnUnequipped()
{
    K2_OnUnequipped();
}
