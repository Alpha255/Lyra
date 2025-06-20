// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerState.h"
#include "GameModes/LyraGameState.h"
#include "GameModes/LyraGameMode.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "LyraLogChannel.h"
#include "Net/UnrealNetwork.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ALyraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (auto World = GetWorld())
	{
		if (World->IsGameWorld() && World->GetNetMode() != NM_Client)
		{
			auto GameState = World->GetGameState();
			check(GameState);

			auto ExperienceMgrComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
			check(ExperienceMgrComp);
			ExperienceMgrComp->OnExperienceLoaded(FLyraOnExperienceLoaded::FDelegate::CreateLambda([this](const ULyraExperienceDefinition*) {
				if (auto GameMode = GetWorld()->GetAuthGameMode<ALyraGameMode>())
				{
					if (auto PawnData = GameMode->GetPawnData(GetOwningController()))
					{
						SetPawnData(PawnData);
					}
					else
					{
						UE_LOG(LogLyra, Error, TEXT("LyraPlayerState::Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
					}
				}
			}), ELyraExperienceLoadPriority::Normal);
		}
	}
}

void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
{
}

void ALyraPlayerState::OnRep_PawnData()
{
}
