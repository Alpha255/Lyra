// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "LyraGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraGameMode : public AModularGameModeBase
{
	GENERATED_BODY()
	
public:
	ALyraGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	void OnSetExperience();
};
