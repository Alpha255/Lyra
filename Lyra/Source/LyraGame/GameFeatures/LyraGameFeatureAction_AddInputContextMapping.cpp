// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/LyraGameFeatureAction_AddInputContextMapping.h"
#include "InputMappingContext.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "GameFramework/PlayerController.h"
#include "Character/LyraHeroComponent.h"
#include "System/LyraAssetManager.h"
#include "Components/GameFrameworkComponentManager.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "GameFeatures"

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddInputContextMapping::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
    
    for (int32 Index = 0; Index < InputMappings.Num(); ++Index)
    {
        if (InputMappings[Index].InputMapping.IsNull())
        {
            Context.AddError(FText::Format(LOCTEXT("InvalidInputMapping", "Input mapping is invalid: {0}"), Index));
            Result = EDataValidationResult::Invalid;
        }
        ++Index;
    }

    return Result;
}
#endif

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureRegistering()
{
    Super::OnGameFeatureRegistering();

    RegisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureUnregistering()
{
    Super::OnGameFeatureUnregistering();

    UnregisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    auto& ActiveData = ContextData.FindOrAdd(Context);
    if (!ensure(ActiveData.ExtRequestHandles.IsEmpty()) ||
        !ensure(ActiveData.ControllersAddedTo.IsEmpty()))
    {
        Reset(ActiveData);
    }

    Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    Super::OnGameFeatureDeactivating(Context);

    auto ActiveData = ContextData.Find(Context);
    if (ensure(ActiveData))
    {
        Reset(*ActiveData);
    }
}

void UGameFeatureAction_AddInputContextMapping::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
    auto World = WorldContext.World();
    auto GameInstance = WorldContext.OwningGameInstance;
    auto& ActiveData = ContextData.FindOrAdd(ChangeContext);

    if (GameInstance && World && World->IsGameWorld())
    {
        if (auto GameFrameworkCompManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
        {
            auto ExtHandleDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleControllerExtension, ChangeContext);
            auto ExtRequestHandle = GameFrameworkCompManager->AddExtensionHandler(APlayerController::StaticClass(), ExtHandleDelegate);

            ActiveData.ExtRequestHandles.Add(ExtRequestHandle);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts()
{
    RegisterIM_Deletage = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts);
    for (auto It = GEngine->GetWorldContexts().CreateConstIterator(); It; ++It)
    {
        RegisterInputMappingContexts(It->OwningGameInstance);
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts()
{
    FWorldDelegates::OnStartGameInstance.Remove(RegisterIM_Deletage);
    RegisterIM_Deletage.Reset();

    for (auto It = GEngine->GetWorldContexts().CreateConstIterator(); It; ++It)
    {
        UnregisterInputMappingContexts(It->OwningGameInstance);
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts(UGameInstance* GameInstance)
{
    if (GameInstance && !GameInstance->OnLocalPlayerAddedEvent.IsBoundToObject(this))
    {
        GameInstance->OnLocalPlayerAddedEvent.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts);
        GameInstance->OnLocalPlayerRemovedEvent.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts);

        for (auto It = GameInstance->GetLocalPlayerIterator(); It; ++It)
        {
            RegisterInputMappingContexts(*It);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts(UGameInstance* GameInstance)
{
    if (GameInstance)
    {
        GameInstance->OnLocalPlayerAddedEvent.RemoveAll(this);
        GameInstance->OnLocalPlayerRemovedEvent.RemoveAll(this);

        for (auto It = GameInstance->GetLocalPlayerIterator(); It; ++It)
        {
            UnregisterInputMappingContexts(GameInstance);
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts(ULocalPlayer* LocalPlayer)
{
    if (!ensure(LocalPlayer))
    {
        return;
    }

    if (auto InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        if (auto UserSettings = InputSystem->GetUserSettings())
        {
            for (auto& InputMapping : InputMappings)
            {
                if (!InputMapping.bRegisterWithSettings)
                {
                    continue;
                }

                if (auto InputMappingContext = ULyraAssetManager::Get().LoadAsset(InputMapping.InputMapping))
                {
                    UserSettings->RegisterInputMappingContext(InputMappingContext);
                }
            }
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts(ULocalPlayer* LocalPlayer)
{
    if (!ensure(LocalPlayer))
    {
        return;
    }

    if (auto InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        if (auto UserSettings = InputSystem->GetUserSettings())
        {
            for (auto& InputMapping : InputMappings)
            {
                if (!InputMapping.bRegisterWithSettings)
                {
                    continue;
                }

                if (auto InputMappingContext = ULyraAssetManager::Get().LoadAsset(InputMapping.InputMapping))
                {
                    UserSettings->UnregisterInputMappingContext(InputMappingContext);
                }
            }
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::Reset(FPerContextData& ActiveData)
{
    ActiveData.ExtRequestHandles.Empty();

    while (!ActiveData.ControllersAddedTo.IsEmpty())
    {
        auto Controller = ActiveData.ControllersAddedTo.Pop();
        if (Controller.IsValid())
        {
            RemoveInputMapping(Controller.Get(), ActiveData);
        }
        else
        {
            ActiveData.ControllersAddedTo.Pop();
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::AddInputMapping(UPlayer* Player, FPerContextData& ActiveData)
{
    if (auto LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            for (auto& InputMapping : InputMappings)
            {
                if (auto InputMappingContext = InputMapping.InputMapping.Get())
                {
                    InputSystem->AddMappingContext(InputMappingContext, InputMapping.Priority);
                }
            }
        }
        else
        {
            UE_LOG(LogGameFeatures, Error, TEXT("Failed to find UEnhancedInputLocalPlayerSubsystem for local player."));
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData)
{
    if (auto LocalPlayer = PlayerController->GetLocalPlayer())
    {
        if (auto InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            for (auto& InputMapping : InputMappings)
            {
                if (auto InputMappingContext = InputMapping.InputMapping.Get())
                {
                    InputSystem->RemoveMappingContext(InputMappingContext);
                }
            }
        }
    }
}

void UGameFeatureAction_AddInputContextMapping::HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
    auto PlayerController = Cast<APlayerController>(Actor);
    auto& ActiveData = ContextData.FindOrAdd(ChangeContext);

    if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
    {
        RemoveInputMapping(PlayerController, ActiveData);
    }
    else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == ULyraHeroComponent::NAME_ReadyToBindInputs)
    {
        AddInputMapping(PlayerController->GetLocalPlayer(), ActiveData);
    }
}

#undef LOCTEXT_NAMESPACE
