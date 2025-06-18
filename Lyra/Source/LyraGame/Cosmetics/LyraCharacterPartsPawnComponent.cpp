// Fill out your copyright notice in the Description page of Project Settings.


#include "Cosmetics/LyraCharacterPartsPawnComponent.h"
#include "Net/UnrealNetwork.h"

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
}

FLyraCharacterPartHandle ULyraCharacterPartsPawnComponent::AddPart(const FLyraCharacterPart& Part)
{
	return PartList.AddEntry(Part);
}

void ULyraCharacterPartsPawnComponent::OnRegister()
{
}

void ULyraCharacterPartsPawnComponent::RemovePart(FLyraCharacterPartHandle Handle)
{
}

void ULyraCharacterPartsPawnComponent::RemoveAllParts()
{

}

void ULyraCharacterPartsPawnComponent::BroadcastPartsChanged()
{
}