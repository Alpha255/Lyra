// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraQuickBarComponent.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"
//#include "GameFramework/GameplayMessageSubsystem.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(LyraGameplayTag_QuickBar_SlotsChanged_Message, "Lyra.QuickBar.Message.SlotsChanged");

ULyraQuickBarComponent::ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void ULyraQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryItemSlots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void ULyraQuickBarComponent::CycleActiveSlotForward()
{
}

void ULyraQuickBarComponent::CycleActiveSlotBackward()
{
}

void ULyraQuickBarComponent::BeginPlay()
{
	if (InventoryItemSlots.Num() < NumSlots)
	{
		InventoryItemSlots.AddDefaulted(NumSlots - InventoryItemSlots.Num());
	}

	Super::BeginPlay();
}

void ULyraQuickBarComponent::OnRep_InventoryItemSlots()
{
	FLyraQuickBarSlotsChangedMessage Message;
	Message.Owner = GetOwner();
	Message.InventoryItemSlots = InventoryItemSlots;

	//UGameplayMessageSubsystem::Get(this).BroadcastMessage(LyraGameplayTag_QuickBar_SlotsChanged_Message, Message);
}

void ULyraQuickBarComponent::OnRep_ActiveSlotIndex()
{
}
