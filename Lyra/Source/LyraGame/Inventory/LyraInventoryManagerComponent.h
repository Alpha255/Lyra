// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LyraInventoryManagerComponent.generated.h"


UCLASS(BlueprintType)
class LYRAGAME_API ULyraInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
