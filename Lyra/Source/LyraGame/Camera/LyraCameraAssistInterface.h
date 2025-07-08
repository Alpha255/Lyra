// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "LyraCameraAssistInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class ULyraCameraAssistInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRAGAME_API ILyraCameraAssistInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetIgnoredActorsForCameraPentration(TArray<const AActor*>& OutActorsAllowPenetration) const {}

	virtual TOptional<AActor*> GetCameraPreventPenetrationTarget() const { return TOptional<AActor*>(); }

	virtual void OnCameraPenetratingTarget() {}
};
