// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayMessageTypes.generated.h"


UENUM(BlueprintType)
enum class EGameplayMessageMatch : uint8
{
	ExactMatch,
	PartialMatch
};

template<class FMessageType>
struct FGameplayMessageListenerParams
{
};
