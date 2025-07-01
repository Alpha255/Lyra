// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "LyraPlayerBotController.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraPlayerBotController : public AModularAIController
{
	GENERATED_BODY()
	
public:
	void ServerRestartController();
};
