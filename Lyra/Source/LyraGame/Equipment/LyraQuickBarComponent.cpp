// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraQuickBarComponent.h"
#include "Net/UnrealNetwork.h"
#include "NativeGameplayTags.h"
#include "GameplayMessageSubsystem.h"
#include "Equipment/LyraEquipmentManagerComponent.h"
#include "Equipment/LyraEquipment.h"
#include "Equipment/LyraEquipmentInstance.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Inventory/LyraInventoryFragment_EquippableItem.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(LyraGameplayTag_QuickBar_SlotsChanged_Message, "Lyra.QuickBar.Message.SlotsChanged");
UE_DEFINE_GAMEPLAY_TAG_STATIC(LyraGameplayTag_QuickBar_ActiveIndexChanged_Message, "Lyra.QuickBar.Message.ActiveIndexChanged");

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
    CycleActiveSlot(1);
}

void ULyraQuickBarComponent::CycleActiveSlotBackward()
{
    CycleActiveSlot(InventoryItemSlots.Num() - 1);
}

void ULyraQuickBarComponent::SetActiveSlotIndex_Implementation(int32 Index)
{
    if (InventoryItemSlots.IsValidIndex(Index) && ActiveSlotIndex != Index)
    {
        UnequipItemInActiveSlot();
        ActiveSlotIndex = Index;
        EquipItemInActiveSlot();
        OnRep_ActiveSlotIndex();
    }
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

	UGameplayMessageSubsystem::Get(this).BroadcastMessage(LyraGameplayTag_QuickBar_SlotsChanged_Message, Message);
}

void ULyraQuickBarComponent::OnRep_ActiveSlotIndex()
{
	FLyraQuickBarActiveSlotChangedMessage Message;
	Message.Owner = GetOwner();
	Message.ActiveSlotIndex = ActiveSlotIndex;

	UGameplayMessageSubsystem::Get(this).BroadcastMessage(LyraGameplayTag_QuickBar_ActiveIndexChanged_Message, Message);
}

void ULyraQuickBarComponent::EquipItemInActiveSlot()
{
    check(InventoryItemSlots.IsValidIndex(ActiveSlotIndex));
    check(EquippedItem == nullptr);

    if (auto SlotItem = InventoryItemSlots[ActiveSlotIndex].Get())
    {
        if (auto InventoryFragment = SlotItem->FindFragmentByClass<ULyraInventoryFragment_EquippableItem>())
        {
            if (auto EquipmentClass = InventoryFragment->Equipment)
            {
                if (auto EquipmentMgrComp = TryGetEquipmentManagerComponent())
                {
                    if (EquippedItem = EquipmentMgrComp->EquipItem(EquipmentClass))
                    {
                        EquippedItem->SetInstigator(SlotItem);
                    }
                }
            }
        }
    }
}

void ULyraQuickBarComponent::UnequipItemInActiveSlot()
{
}

void ULyraQuickBarComponent::CycleActiveSlot(int32 Factor)
{
    if (InventoryItemSlots.Num() < 2)
    {
        return;
    }

    const int32 OldIndex = ActiveSlotIndex < 0 ? InventoryItemSlots.Num() - 1 : ActiveSlotIndex;
    int32 NewIndex = OldIndex;
    do
    {
        NewIndex = (NewIndex + Factor) % InventoryItemSlots.Num();
        if (InventoryItemSlots[NewIndex])
        {
            SetActiveSlotIndex(NewIndex);
            break;
        }
    } while (NewIndex != OldIndex);
}

ULyraEquipmentManagerComponent* ULyraQuickBarComponent::TryGetEquipmentManagerComponent() const
{
    if (auto Controller = Cast<AController>(GetOwner()))
    {
        if (auto Pawn = Controller->GetPawn())
        {
            return Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>();
        }
    }

    return nullptr;
}
