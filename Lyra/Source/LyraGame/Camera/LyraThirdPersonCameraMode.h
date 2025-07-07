// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "LyraThirdPersonCameraMode.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LYRAGAME_API ULyraThirdPersonCameraMode : public ULyraCameraMode
{
	GENERATED_BODY()
	
public:
	ULyraThirdPersonCameraMode();
protected:
	virtual void UpdateView(float DeltaTime) override;
};
