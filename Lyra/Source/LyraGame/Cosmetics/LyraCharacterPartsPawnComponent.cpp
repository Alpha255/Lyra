// Fill out your copyright notice in the Description page of Project Settings.


#include "Cosmetics/LyraCharacterPartsPawnComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

ULyraCharacterPartsPawnComponent::ULyraCharacterPartsPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void ULyraCharacterPartsPawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PartList);
}

void ULyraCharacterPartsPawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PartList.RemoveAllEntries(false);
	Super::EndPlay(EndPlayReason);
}

FLyraCharacterPartHandle ULyraCharacterPartsPawnComponent::AddPart(const FLyraCharacterPart& Part)
{
	return PartList.AddEntry(Part);
}

void ULyraCharacterPartsPawnComponent::OnRegister()
{
	Super::OnRegister();

	if (!IsTemplate())
	{
		PartList.SetOwner(this);
	}
}

void ULyraCharacterPartsPawnComponent::RemovePart(FLyraCharacterPartHandle Handle)
{
	PartList.RemoveEntry(Handle);
}

void ULyraCharacterPartsPawnComponent::RemoveAllParts()
{
	PartList.RemoveAllEntries(true);
}

void ULyraCharacterPartsPawnComponent::BroadcastPartsChanged()
{
	OnPartsChanged.Broadcast(this);
}

USkeletalMeshComponent* ULyraCharacterPartsPawnComponent::GetSkeletalMeshComponent() const
{
	if (auto Owner = Cast<ACharacter>(GetOwner()))
	{
		return Owner->GetMesh();
	}

	return nullptr;
}

USceneComponent* ULyraCharacterPartsPawnComponent::GetSceneComponentAttachTo() const
{
	if (auto SkeletalMeshComp = GetSkeletalMeshComponent())
	{
		return SkeletalMeshComp;
	}
	else if (auto Owner = GetOwner())
	{
		return Owner->GetRootComponent();
	}

	return nullptr;
}
