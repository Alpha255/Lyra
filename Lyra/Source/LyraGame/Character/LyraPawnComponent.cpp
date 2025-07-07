// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraPawnComponent.h"
#include "Character/LyraPawnData.h"
#include "LyraGameplayTags.h"
#include "LyraLogChannel.h"
#include "Net/UnrealNetwork.h"
#include "Components/GameFrameworkComponentManager.h"

const FName ULyraPawnComponent::NAME_Feature(TEXT("LyraPawnComp"));

ULyraPawnComponent::ULyraPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

bool ULyraPawnComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	auto Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == LyraGameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == LyraGameplayTags::InitState_Spawned && DesiredState == LyraGameplayTags::InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}

		if ((Pawn->HasAuthority() || Pawn->IsLocallyControlled()) && !GetController<AController>())
		{
			return false;
		}

		return true;
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataAvailable && DesiredState == LyraGameplayTags::InitState_DataInitialized)
	{
		return Manager->HasFeatureReachedInitState(Pawn, NAME_Feature, LyraGameplayTags::InitState_DataAvailable);
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataInitialized && DesiredState == LyraGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraPawnComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_Feature && Params.FeatureState == LyraGameplayTags::InitState_DataAvailable)
	{
		CheckDefaultInitialization();
	}
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

void ULyraPawnComponent::OnControllerChanged()
{
	CheckDefaultInitialization();
}

void ULyraPawnComponent::OnPlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void ULyraPawnComponent::OnSetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void ULyraPawnComponent::OnRegister()
{
	Super::OnRegister();

	RegisterInitStateFeature();
}

void ULyraPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	ensure(TryToChangeInitState(LyraGameplayTags::InitState_Spawned));

	CheckDefaultInitialization();
}

void ULyraPawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}
