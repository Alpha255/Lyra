// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "GameUIManagerSubsystem.generated.h"

UCLASS(Abstract, config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
};