// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraEquipment.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class LYRAGAME_API ULyraEquipment : public UObject
{
	GENERATED_BODY()

public:
    ULyraEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, Category = "Equipment")
    TSubclassOf<class ULyraEquipmentInstance> InstanceClass;
};
