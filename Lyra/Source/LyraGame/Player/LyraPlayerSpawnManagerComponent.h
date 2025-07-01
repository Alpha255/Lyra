// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraPlayerSpawnManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraPlayerSpawnManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
protected:
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) {}

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName=OnFinishRestartPlayer))
	void K2_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);
private:
	friend class ALyraGameMode;

	AActor* ChoosePlayerStart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);

#if WITH_EDITOR
	APlayerStart* FindPlayerStart(AController* Player);
#endif
};
