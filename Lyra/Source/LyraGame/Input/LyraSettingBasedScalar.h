// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "LyraSettingBasedScalar.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, MinimalAPI, Meta=(DisplayName="Lyra Setting Based Scalar"))
class ULyraSettingBasedScalar : public UInputModifier
{
	GENERATED_BODY()

public:

};

UENUM()
enum class EDeadZoneStick : uint8
{
	MoveStick,
	LookStick
};

UCLASS(NotBlueprintable, MinimalAPI, Meta=(DisplayName = "Lyra DeadZone"))
class ULyraDeadZone : public UInputModifier
{
	GENERATED_BODY()

public:
};

UCLASS(NotBlueprintable, MinimalAPI, Meta=(DisplayName = "Lyra Gamepad Sensitivity"))
class ULyraGamepadSensitivity : public UInputModifier
{
	GENERATED_BODY()

public:
};

UCLASS(NotBlueprintable, MinimalAPI, Meta=(DisplayName = "Lyra Aim Inversion"))
class ULyraAimInversion : public UInputModifier
{
	GENERATED_BODY()

public:
};