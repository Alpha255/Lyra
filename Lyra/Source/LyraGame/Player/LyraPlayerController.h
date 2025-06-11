// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
