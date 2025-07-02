// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerStart.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/GameModeBase.h"

ELyraPlayerStartOccupancyState ALyraPlayerStart::GetOccupancyState(AController* Controller) const
{
	auto World = GetWorld();
	if (HasAuthority() && World)
	{
		if (auto GameMode = World->GetAuthGameMode())
		{
			TSubclassOf<APawn> PawnClass = GameMode->GetDefaultPawnClassForController(Controller);
			auto Pawn = PawnClass ? GetDefault<APawn>(PawnClass) : nullptr;

			FVector ActorLocation = GetActorLocation();
			FRotator ActorRotation = GetActorRotation();

			if (!World->EncroachingBlockingGeometry(Pawn, ActorLocation, ActorRotation, nullptr))
			{
				return ELyraPlayerStartOccupancyState::Empty;
			}
			else if (World->FindTeleportSpot(Pawn, ActorLocation, ActorRotation))
			{
				return ELyraPlayerStartOccupancyState::Partial;
			}
		}
	}

	return ELyraPlayerStartOccupancyState::Full;
}

bool ALyraPlayerStart::IsOccupied() const
{
	return OccupiedBy != nullptr;
}

bool ALyraPlayerStart::TryOccupy(AController* Controller)
{
	if (Controller && !IsOccupied())
	{
		OccupiedBy = Controller;
		
		if (auto World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ExpirationCheckTimer, FTimerDelegate::CreateUObject(this, &ALyraPlayerStart::CheckUnOccupied), ExpirationCheckInterval, true);
		}

		return true;
	}

	return false;
}

void ALyraPlayerStart::CheckUnOccupied()
{
	if (OccupiedBy && OccupiedBy->GetPawn() && GetOccupancyState(OccupiedBy) == ELyraPlayerStartOccupancyState::Empty)
	{
		OccupiedBy = nullptr;

		if (auto World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ExpirationCheckTimer);
		}
	}
}
