// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraEquipmentManagerComponent.generated.h"

class ULyraEquipmentInstance;

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ULyraEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<ULyraEquipmentInstance> EquipmentType) const;
};
