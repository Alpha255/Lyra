// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraQuickBarComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Lyra")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category = "Lyra")
	void CycleActiveSlotBackward();

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Lyra")
    void SetActiveSlotIndex(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<ULyraInventoryItemInstance*> GetInventoryItemSlots() const { return InventoryItemSlots; }

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void AddInventoryItemToSlot(int32 Index, ULyraInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	virtual void BeginPlay() override;
protected:
	UPROPERTY()
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_InventoryItemSlots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();
private:
    void EquipItemInActiveSlot();
    void UnequipItemInActiveSlot();

    void CycleActiveSlot(int32 Factor);

    class ULyraEquipmentManagerComponent* TryGetEquipmentManagerComponent() const;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItemSlots)
	TArray<TObjectPtr<class ULyraInventoryItemInstance>> InventoryItemSlots;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<class ULyraEquipmentInstance> EquippedItem;
};


USTRUCT(BlueprintType)
struct FLyraQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<class ULyraInventoryItemInstance>> InventoryItemSlots;
};

USTRUCT(BlueprintType)
struct FLyraQuickBarActiveSlotChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 ActiveSlotIndex = -1;
};
