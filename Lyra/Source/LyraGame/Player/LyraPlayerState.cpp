// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerState.h"
#include "GameModes/LyraGameState.h"
#include "GameModes/LyraGameMode.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Character/LyraPawnData.h"
#include "LyraLogChannel.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/LyraGameplayAbilitySet.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    AbilitySysComp = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
    AbilitySysComp->SetIsReplicated(true);
    AbilitySysComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // AbilitySystemComponent needs to be updated at a high frequency.
    SetNetUpdateFrequency(100.0f);
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
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLyra, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."),
			*GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
	}
	else
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
		PawnData = InPawnData;

        for (const auto& AbilitySet : PawnData->GameplayAbilitySets)
        {
            if (AbilitySet)
            {
                AbilitySet->GiveAbility(AbilitySysComp, nullptr);
            }
        }

		ForceNetUpdate();
	}
}

void ALyraPlayerState::OnRep_PawnData()
{
}
