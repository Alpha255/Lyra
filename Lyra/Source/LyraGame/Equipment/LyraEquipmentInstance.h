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

private:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
