// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LyraHeroComponent.h"
#include "LyraGameplayTags.h"
#include "LyraLogChannel.h"
#include "Character/LyraPawnComponent.h"
#include "Character/LyraPawnData.h"
#include "Player/LyraPlayerState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraLocalPlayer.h"
#include "Camera/LyraCameraComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "InputMappingContext.h"
#include "PlayerMappableInputConfig.h"
#include "Input/LyraInputConfig.h"
#include "Input/LyraInputComponent.h"
#include "Character/LyraCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

const FName ULyraHeroComponent::NAME_Feature(TEXT("LyraHeroComp"));
const FName ULyraHeroComponent::NAME_ReadyToBindInputs(TEXT("LyraHeroCompReadeToBindInputs"));

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
			UE_LOG(LogLyra, Log, TEXT("Gameplay state can change from [%s] to [%s]"), *CurrentState.ToString(), *DesiredState.ToString());
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

		UE_LOG(LogLyra, Log, TEXT("Gameplay state can change from [%s] to [%s]"), *CurrentState.ToString(), *DesiredState.ToString());
		return true;
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataAvailable && DesiredState == LyraGameplayTags::InitState_DataInitialized)
	{
		UE_LOG(LogLyra, Log, TEXT("Gameplay state can change from [%s] to [%s]"), *CurrentState.ToString(), *DesiredState.ToString());
		auto PlayerState = GetPlayerState<ALyraPlayerState>();
		return PlayerState && Manager->HasFeatureReachedInitState(Pawn, ULyraPawnComponent::NAME_Feature, LyraGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == LyraGameplayTags::InitState_DataInitialized && DesiredState == LyraGameplayTags::InitState_GameplayReady)
	{
		UE_LOG(LogLyra, Log, TEXT("Gameplay state can change from [%s] to [%s]"), *CurrentState.ToString(), *DesiredState.ToString());
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
    check(PlayerInputComponent);

    const auto Pawn = GetPawn<APawn>();
    if (!Pawn)
    {
        return;
    }

    const auto PlayerController = GetController<ALyraPlayerController>();
    check(PlayerController);

    const auto LocalPlayer = Cast<ULyraLocalPlayer>(PlayerController->GetLocalPlayer());
    check(LocalPlayer);

    auto EnhancedInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(EnhancedInputSubsystem);

    EnhancedInputSubsystem->ClearAllMappings();
    if (const auto PawnComp = ULyraPawnComponent::GetPawnComponent(Pawn))
    {
        if (const auto PawnData = PawnComp->GetPawnData())
        {
            if (auto InputConfig = PawnData->InputConfig)
            {
                for (auto& InputMappingContext : DefaultInputMappingContexts)
                {
                    if (auto InputMapping = InputMappingContext.InputMapping.Get())
                    {
                        if (InputMappingContext.bRegisterWithSettings)
                        {
                            if (auto InputUserSettings = EnhancedInputSubsystem->GetUserSettings())
                            {
                                InputUserSettings->RegisterInputMappingContext(InputMapping);
                            }

                            FModifyContextOptions Options{};
                            Options.bIgnoreAllPressedKeysUntilRelease = false;
                            EnhancedInputSubsystem->AddMappingContext(InputMapping, InputMappingContext.Priority, Options);
                        }
                    }
                }

                auto LyraInputComp = Cast<ULyraInputComponent>(PlayerInputComponent);
                if (ensure(LyraInputComp))
                {
                    TArray<uint32> BindHandles;

                    LyraInputComp->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityTagPressed, &ThisClass::Input_AbilityTagReleased, BindHandles);

                    LyraInputComp->BindNativeAction(InputConfig, LyraGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
                    LyraInputComp->BindNativeAction(InputConfig, LyraGameplayTags::InputTag_LookMouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
                    LyraInputComp->BindNativeAction(InputConfig, LyraGameplayTags::InputTag_LookStick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, false);
                    LyraInputComp->BindNativeAction(InputConfig, LyraGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, false);
                    LyraInputComp->BindNativeAction(InputConfig, LyraGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, false);
                }
            }
        }
    }

    bReadyToBindInputs = true;

    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(PlayerController, NAME_ReadyToBindInputs);
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(Pawn, NAME_ReadyToBindInputs);
}

void ULyraHeroComponent::Input_AbilityTagPressed(FGameplayTag Tag)
{
}

void ULyraHeroComponent::Input_AbilityTagReleased(FGameplayTag Tag)
{
}

void ULyraHeroComponent::Input_Move(const FInputActionValue& ActionValue)
{
    auto Pawn = GetPawn<APawn>();
    auto Controller = Pawn ? GetController<ALyraPlayerController>() : nullptr;

    if (Controller)
    {
        const FVector2D MoveValue = ActionValue.Get<FVector2D>();
        const FRotator Rotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

        if (MoveValue.X != 0.0f)
        {
            Pawn->AddMovementInput(Rotation.RotateVector(FVector::RightVector), MoveValue.X);
        }

        if (MoveValue.Y != 0.0f)
        {
            Pawn->AddMovementInput(Rotation.RotateVector(FVector::ForwardVector), MoveValue.Y);
        }
    }
}

void ULyraHeroComponent::Input_LookMouse(const FInputActionValue& ActionValue)
{
    if (auto Pawn = GetPawn<APawn>())
    {
        const FVector2D LookValue = ActionValue.Get<FVector2D>();

        if (LookValue.X != 0.0f)
        {
            Pawn->AddControllerYawInput(LookValue.X);
        }

        if (LookValue.Y != 0.0f)
        {
            Pawn->AddControllerPitchInput(LookValue.Y);
        }
    }
}

void ULyraHeroComponent::Input_LookStick(const FInputActionValue& ActionValue)
{
    static const float LookYawRate = 300.0f;
    static const float LookPitchRate = 165.0f;

    if (auto Pawn = GetPawn<APawn>())
    {
        const FVector2D LookValue = ActionValue.Get<FVector2D>();

        auto World = GetWorld();
        check(World);

        if (LookValue.X != 0.0f)
        {
            Pawn->AddControllerYawInput(LookValue.X * LookYawRate * World->GetDeltaSeconds());
        }

        if (LookValue.Y != 0.0f)
        {
            Pawn->AddControllerPitchInput(LookValue.Y * LookPitchRate * World->GetDeltaSeconds());
        }
    }
}

void ULyraHeroComponent::Input_Crouch(const FInputActionValue& ActionValue)
{
    if (auto Character = GetPawn<ALyraCharacter>())
    {
        Character->ToggleCrouch();
    }
}

void ULyraHeroComponent::Input_AutoRun(const FInputActionValue& ActionValue)
{
    if (auto Pawn = GetPawn<APawn>())
    {
        if (auto Controller = GetController<ALyraPlayerController>())
        {

        }
    }
}
