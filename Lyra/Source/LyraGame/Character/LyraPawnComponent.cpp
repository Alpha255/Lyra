// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraPawnComponent.h"
#include "Character/LyraPawnData.h"
#include "LyraGameplayTags.h"
#include "LyraLogChannel.h"
#include "Net/UnrealNetwork.h"

const FName ULyraPawnComponent::NAME_Feature(TEXT("LyraPawnComp"));

ULyraPawnComponent::ULyraPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

void ULyraPawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Replicate PawnData to clients
	DOREPLIFETIME(ULyraPawnComponent, PawnData);
}

void ULyraPawnComponent::CheckDefaultInitialization()
{
	IGameFrameworkInitStateInterface::CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StatChain =
	{
		LyraGameplayTags::InitState_Spawned,
		LyraGameplayTags::InitState_DataAvailable,
		LyraGameplayTags::InitState_DataInitialized,
		LyraGameplayTags::InitState_GameplayReady
	};

	IGameFrameworkInitStateInterface::ContinueInitStateChain(StatChain);
}

void ULyraPawnComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void ULyraPawnComponent::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	auto Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLyra, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), 
			*GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
	}
	else
	{
		PawnData = InPawnData;
		Pawn->ForceNetUpdate();

		CheckDefaultInitialization();
	}
}