// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_Base.h"
#include "GFA_AddInputContextMapping.generated.h"

/**
 * 
 */

USTRUCT()
struct FLyraInputMappingContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input", Meta = (AssetBundles = "Client,Server"))
    TSoftObjectPtr<class UInputMappingContext> InputMapping;

    UPROPERTY(EditAnywhere, Category = "Input")
    int32 Priority = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    bool bRegisterWithSettings = true;
};

UCLASS(MinimalAPI)
class UGFA_AddInputContextMapping : public UGameFeatureAction_Base
{
	GENERATED_BODY()
	
};
