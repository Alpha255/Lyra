// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "LyraGameState.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraGameState : public AModularGameStateBase
{
	GENERATED_BODY()
	
public:
	ALyraGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:

	TObjectPtr<class ULyraExperienceManagerComponent> ExperienceMgrComp;
};
