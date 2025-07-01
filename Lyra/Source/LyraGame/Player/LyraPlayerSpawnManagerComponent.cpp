// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerSpawnManagerComponent.h"
#include "Player/LyraPlayerStart.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"

AActor* ULyraPlayerSpawnManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{
#if WITH_EDITOR
		if (auto PlayerStart = FindPlayerStart(Player))
		{
			return PlayerStart;
		}
#endif

		TArray<ALyraPlayerStart*> PlayerStarts;
	}

	return nullptr;
}

void ULyraPlayerSpawnManagerComponent::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	OnFinishRestartPlayer(NewPlayer, StartRotation);
	K2_OnFinishRestartPlayer(NewPlayer, StartRotation);
}

#if WITH_EDITOR
APlayerStart* ULyraPlayerSpawnManagerComponent::FindPlayerStart(AController* Player)
{
	if (Player && Player->IsA<APlayerController>())
	{
		if (auto World = GetWorld())
		{
			for (TActorIterator<APlayerStart> It(World); It; ++It)
			{
				if (auto PlayerStart = *It)
				{
					if (PlayerStart->IsA<APlayerStartPIE>())
					{
						return PlayerStart;
					}
				}
			}
		}
	}

	return nullptr;
}
#endif
