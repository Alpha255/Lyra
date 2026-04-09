// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameUIManagerSubsystem.h"
#include "LyraWidgetsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraWidgetsSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:
    ULyraWidgetsSubsystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
};
