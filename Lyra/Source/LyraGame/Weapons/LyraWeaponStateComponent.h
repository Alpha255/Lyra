// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraWeaponStateComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraWeaponStateComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
    ULyraWeaponStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    double GetTimeSinceLastHitNotification() const;

private:
    double LastWeaponDamageInsitagatedTime = 0.0f;
};
