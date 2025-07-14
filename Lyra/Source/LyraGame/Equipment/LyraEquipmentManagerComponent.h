// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraEquipmentManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
    ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    class ULyraEquipmentInstance* EquipItem(TSubclassOf<class ULyraEquipment> EquipmentClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<class ULyraEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const;
};
