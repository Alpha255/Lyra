// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerSpawnManagerComponent.h"
#include "Player/LyraPlayerStart.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerState.h"

ULyraPlayerSpawnManagerComponent::ULyraPlayerSpawnManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULyraPlayerSpawnManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ThisClass::OnLevelAddedToWorld);

	if (auto World = GetWorld())
	{
		World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::OnActorSpawned));

		for (TActorIterator<ALyraPlayerStart> It(World); It; ++It)
		{
			if (*It)
			{
				PlayerStarts.AddUnique(*It);
			}
		}
	}
}

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

		TArray<ALyraPlayerStart*> ValidPlayerStarts;
		for (auto It = PlayerStarts.CreateIterator(); It; ++It)
		{
			if (auto PlayerStart = It->Get())
			{
				ValidPlayerStarts.Add(PlayerStart);
			}
			else
			{
				It.RemoveCurrent();
			}
		}

		if (auto PlayerState = Player->GetPlayerState<APlayerState>())
		{
			if (PlayerState->IsOnlyASpectator())
			{
				if (ValidPlayerStarts.IsEmpty())
				{
					return ValidPlayerStarts[FMath::RandRange(0, ValidPlayerStarts.Num() - 1)];
				}
				else
				{
					return nullptr;
				}
			}
		}

		auto PlayerStart = ChoosePlayerStart(Player, ValidPlayerStarts);
		if (!PlayerStart)
		{
			PlayerStart = GetRandomPlayerStart(Player, ValidPlayerStarts);
		}

		if (PlayerStart)
		{
			PlayerStart->TryOccupy(Player);
		}

		return PlayerStart;
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

void ULyraPlayerSpawnManagerComponent::OnLevelAddedToWorld(ULevel* Level, UWorld* World)
{
	if (World == GetWorld())
	{
		for (auto Actor : Level->Actors)
		{
			if (auto PlayerStart = Cast<ALyraPlayerStart>(Actor))
			{
				PlayerStarts.AddUnique(PlayerStart);
			}
		}
	}
}

void ULyraPlayerSpawnManagerComponent::OnActorSpawned(AActor* SpawnedActor)
{
	if (auto PlayerStart = Cast<ALyraPlayerStart>(SpawnedActor))
	{
		PlayerStarts.AddUnique(PlayerStart);
	}
}

ALyraPlayerStart* ULyraPlayerSpawnManagerComponent::ChoosePlayerStart(AController* Player, TArray<class ALyraPlayerStart*>& ValidPlayerStarts)
{
	return nullptr;
}

ALyraPlayerStart* ULyraPlayerSpawnManagerComponent::GetRandomPlayerStart(AController* Player, TArray<class ALyraPlayerStart*>& ValidPlayerStarts)
{
	if (Player)
	{
		TArray<ALyraPlayerStart*> FreePlayerStarts;
		TArray<ALyraPlayerStart*> OccupiedPlayerStarts;

		for (auto PlayerStart : ValidPlayerStarts)
		{
			switch (PlayerStart->GetOccupancyState(Player))
			{
			case ELyraPlayerStartOccupancyState::Empty:
				FreePlayerStarts.Add(PlayerStart);
				break;
			case ELyraPlayerStartOccupancyState::Partial:
				OccupiedPlayerStarts.Add(PlayerStart);
				break;
			}
		}

		if (!FreePlayerStarts.IsEmpty())
		{
			return FreePlayerStarts[FMath::RandRange(0, FreePlayerStarts.Num() - 1)];
		}
		else if (!OccupiedPlayerStarts.IsEmpty())
		{
			return OccupiedPlayerStarts[FMath::RandRange(0, OccupiedPlayerStarts.Num() - 1)];
		}
	}

	return nullptr;
}
