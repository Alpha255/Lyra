// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraGameMode.h"
#include "Character/LyraCharacter.h"
#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "GameModes/LyraWorldSettings.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameModes/LyraExperienceDefinition.h"
#include "System/LyraAssetManager.h"
#include "LyraLogChannel.h"

ALyraGameMode::ALyraGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ALyraGameState::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
}

void ALyraGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::SetGameplayExperience);
}

UClass* ALyraGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return nullptr;
}

APawn* ALyraGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return nullptr;
}

bool ALyraGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void ALyraGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
}

AActor* ALyraGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return nullptr;
}

void ALyraGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
}

bool ALyraGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return false;
}

void ALyraGameMode::InitGameState()
{
}

bool ALyraGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return false;
}

void ALyraGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
}

void ALyraGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
}

const ULyraPawnData* ALyraGameMode::GetPawnData(const AController* Controller) const
{
	if (Controller)
	{
		if (auto PlayerState = Controller->GetPlayerState<ALyraPlayerState>())
		{
			if (auto PawnData = PlayerState->GetPawnData())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	auto ExperienceMgrComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceMgrComp);

	if (ExperienceMgrComp->IsExperienceLoaded())
	{
		auto Experience = ExperienceMgrComp->GetCurrentExperience();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		return ULyraAssetManager::Get().GetDefaultPawnData();
	}

	return nullptr;
}

void ALyraGameMode::SetGameplayExperience()
{
	if (auto WorldSettings = Cast<ALyraWorldSettings>(GetWorldSettings()))
	{
		auto AssetId = WorldSettings->GetGameplayExperience();
		if (AssetId.IsValid())
		{
			if (auto ExperienceMgrComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>())
			{
				ExperienceMgrComp->SetExperience(AssetId);
			}
			else
			{
				UE_LOG(LogLyra, Error, TEXT("Failed to get experience manager component"));
			}
		}
		else
		{
			UE_LOG(LogLyra, Error, TEXT("Failed to identify experience"));
		}
	}
}
