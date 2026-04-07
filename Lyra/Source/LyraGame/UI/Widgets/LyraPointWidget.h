// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicEntryBoxBase.h"
#include "LyraPointWidget.generated.h"

UENUM(BlueprintType)
enum class EPointMatch : uint8
{
    ExactMatch,
    PartialMatch
};

UENUM(BlueprintType)
enum class EAction : uint8
{
    Added,
    Removed
};

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
	
};
