// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraEquipmentInstance : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category="Equipment")
	TArray<AActor*> GetSpawnActors() const { return SpawnedActors; }

    UFUNCTION(BlueprintPure, Category = "Equipment")
    UObject* GetInstigator() const { return Instigator; }

    void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

    virtual void OnEquipped();
    virtual void OnUnequipped();
protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", Meta = (DisplayName = "OnEquipped"))
    void K2_OnEquipped();

    UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", Meta = (DisplayName = "OnUnequipped"))
    void K2_OnUnequipped();
private:
    UPROPERTY(Replicated)
    TObjectPtr<UObject> Instigator;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
