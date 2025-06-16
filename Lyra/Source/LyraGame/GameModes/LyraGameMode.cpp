// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraGameMode.h"
#include "Character/LyraCharacter.h"
#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerState.h"
#include "GameModes/LyraWorldSettings.h"
#include "GameModes/LyraExperienceManagerComponent.h"
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

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::OnSetExperience);
}

void ALyraGameMode::OnSetExperience()
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
