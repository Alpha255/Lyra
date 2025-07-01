// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraGameMode.h"
#include "Character/LyraCharacter.h"
#include "Character/LyraPawnData.h"
#include "Character/LyraPawnComponent.h"
#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "Player/LyraPlayerSpawnManagerComponent.h"
#include "Player/LyraPlayerBotController.h"
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
	if (auto PawnData = GetPawnData(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController(InController);
}

APawn* ALyraGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.ObjectFlags |= RF_Transient;
	SpawnParams.bDeferConstruction = true;

	if (auto PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (auto SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnParams))
		{
			if (auto PawnComp = ULyraPawnComponent::GetPawnComponent(SpawnedPawn))
			{
				if (auto PawnData = GetPawnData(NewPlayer))
				{
					PawnComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogLyra, Error, TEXT("LyraGameMode: Unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogLyra, Error, TEXT("LyraGameMode: Unable to spawn pawn of class [%s] at [%s]"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogLyra, Error, TEXT("LyraGameMode: Unable to spawn pawn due to null pawn class"));
	}

	return nullptr;
}

bool ALyraGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void ALyraGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	check(GameState);
	auto ExperienceMgrComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceMgrComp);

	if (ExperienceMgrComp->IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* ALyraGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (auto PlayerSpawnMgrComp = GameState->FindComponentByClass<ULyraPlayerSpawnManagerComponent>())
	{
		return PlayerSpawnMgrComp->ChoosePlayerStart(Player);
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ALyraGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	if (auto PlayerSpawnMgrComp = GameState->FindComponentByClass<ULyraPlayerSpawnManagerComponent>())
	{
		return PlayerSpawnMgrComp->FinishRestartPlayer(NewPlayer, StartRotation);
	}

	return Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ALyraGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	if (auto PlayerController = Cast<APlayerController>(Player))
	{
		if (!Super::PlayerCanRestart_Implementation(PlayerController))
		{
			return false;
		}
	}
	else
	{
		if (!Player || Player->IsPendingKillPending())
		{
			return false;
		}
	}

	// PlayerSpawningManagerComp
	if (auto PlayerSpawnMgrComp = GameState->FindComponentByClass<ULyraPlayerSpawnManagerComponent>())
	{
		return true;
	}

	return false;
}

void ALyraGameMode::InitGameState()
{
	Super::InitGameState();

	auto ExperienceMgrComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceMgrComp);
	ExperienceMgrComp->OnExperienceLoaded(FLyraOnExperienceLoaded::FDelegate::CreateLambda([this](const ULyraExperienceDefinition* Experience) {
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			auto PlayerController = Cast<APlayerController>(*It);
			if (PlayerController && !PlayerController->GetPawn())
			{
				if (PlayerCanRestart(PlayerController))
				{
					RestartPlayer(PlayerController);
				}
			}
		}
	}), ELyraExperienceLoadPriority::Normal);
}

bool ALyraGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	return true;
}

void ALyraGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnLyraPlayerInitialized.Broadcast(this, NewPlayer);
}

void ALyraGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	if (auto PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (auto Controller = Cast<APlayerController>(NewPlayer))
		{
			if (PlayerCanRestart(Controller))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);
			}
			else
			{
				UE_LOG(LogLyra, Log, TEXT("LyraGameMode: Failed to restart player (%s)"), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(LogLyra, Log, TEXT("LyraGameMode: Failed to restart player (%s)"), *GetPathNameSafe(NewPlayer));
	}
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

void ALyraGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && Controller)
	{
		Controller->Reset();
	}

	if (auto PlayerController = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PlayerController, &APlayerController::ServerRestartPlayer_Implementation);
	}
	else if (auto BotController = Cast<ALyraPlayerBotController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(BotController, &ALyraPlayerBotController::ServerRestartController);
	}
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
