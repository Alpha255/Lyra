// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraCameraMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELyraCameraModeBlendFunction :uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	Num UMETA(Hidden)
};

struct FLyraCameraModeView
{
	FLyraCameraModeView();

	void Blend(const FLyraCameraModeView& Other, float Weight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UCLASS(Abstract, NotBlueprintable)
class LYRAGAME_API ULyraCameraMode : public UObject
{
	GENERATED_BODY()
	
};
