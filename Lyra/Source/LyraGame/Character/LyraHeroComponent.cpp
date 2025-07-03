// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraHeroComponent.h"
#include "LyraGameplayTags.h"
#include "LyraLogChannel.h"
#include "Character/LyraPawnComponent.h"
#include "Character/LyraPawnData.h"
#include "Player/LyraPlayerState.h"
#include "Player/LyraPlayerController.h"
#include "Camera/LyraCameraComponent.h"
#include "Components/GameFrameworkComponentManager.h"

const FName ULyraHeroComponent::NAME_Feature(TEXT("LyraHeroComp"));

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ULyraHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
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
	else if (CurrentState == LyraGameplayTags::InitState_Spawned && DesiredState == LyraGameplayTags::InitState_DataAvailable)
	{
		if (!GetPlayerState<ALyraPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy) // Controled by local player or on the server
		{
			auto Controller = GetController<AController>();
			if (!(Controller && Controller->PlayerState && Controller->PlayerState->GetOwner() == Controller))
			{
				return false;
			}
		}

		if (Pawn->IsLocallyControlled() && !Pawn->IsBotControlled())
		{
			auto Controller = GetController<ALyraPlayerController>();
			if (!Pawn->InputComponent || !Controller || !Controller->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataAvailable && DesiredState == LyraGameplayTags::InitState_DataInitialized)
	{
		auto PlayerState = GetPlayerState<ALyraPlayerState>();
		return PlayerState && Manager->HasFeatureReachedInitState(Pawn, ULyraPawnComponent::NAME_Feature, LyraGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataInitialized && DesiredState == LyraGameplayTags::InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void ULyraHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == LyraGameplayTags::InitState_DataAvailable && DesiredState == LyraGameplayTags::InitState_DataInitialized)
	{
		auto Pawn = GetPawn<APawn>();
		auto PlayerState = GetPlayerState<ALyraPlayerState>();
		if (!ensure(Pawn && PlayerState))
		{
			return;
		}

		const ULyraPawnData* PawnData = nullptr;

		if (auto LyraPawnComp = ULyraPawnComponent::GetPawnComponent(Pawn))
		{
			PawnData = LyraPawnComp->GetPawnData();
		}

		if (auto Controller = GetController<ALyraPlayerController>())
		{
			if (Pawn->InputComponent)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if (PawnData)
		{
			if (auto CameraComp = ULyraCameraComponent::GetCameraComponent(Pawn))
			{
				CameraComp->DetermineCameModeDelegate.BindLambda([this]() -> TSubclassOf<ULyraCameraMode> 
					{
						auto Pawn = GetPawn<APawn>();
						if (!Pawn)
						{
							return nullptr;
						}

						if (auto PawnComp = ULyraPawnComponent::GetPawnComponent(Pawn))
						{
							if (auto PawnData = PawnComp->GetPawnData())
							{
								return PawnData->DefaultCameraMode;
							}
						}

						return nullptr;
					});
			}
		}
	}
}

void ULyraHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == ULyraPawnComponent::NAME_Feature &&
		Params.FeatureState == LyraGameplayTags::InitState_DataInitialized)
	{
		CheckDefaultInitialization();
	}
}

void ULyraHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain
	{
		LyraGameplayTags::InitState_Spawned,
		LyraGameplayTags::InitState_DataAvailable,
		LyraGameplayTags::InitState_DataInitialized,
		LyraGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

void ULyraHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLyra, Error, TEXT("LyraHeroComponent:: This component has been added to a blueprint whose base class is not a Pawn."));
	}
	else
	{
		RegisterInitStateFeature();
	}
}

void ULyraHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	BindOnActorInitStateChanged(ULyraPawnComponent::NAME_Feature, FGameplayTag(), false);

	ensure(TryToChangeInitState(LyraGameplayTags::InitState_Spawned));

	CheckDefaultInitialization();
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void ULyraHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
}
