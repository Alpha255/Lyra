// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraGameMode.h"
#include "Character/LyraCharacter.h"
#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"

ALyraGameMode::ALyraGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ALyraGameState::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
}
