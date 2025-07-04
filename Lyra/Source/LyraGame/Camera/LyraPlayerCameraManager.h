// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "LyraPlayerCameraManager.generated.h"

/**
 * 
 */

#define LYRA_CAMERA_DEFAULT_FOV (80.0f)
#define LYRA_CAMERA_DEFAULT_PITCH_MIN (-90.0f)
#define LYRA_CAMERA_DEFAULT_PITCH_MAX (90.0f)


UCLASS(NotPlaceable, MinimalAPI)
class ALyraPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
};
