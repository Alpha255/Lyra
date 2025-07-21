// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GFA_AddInputContextMapping.h"

#if WITH_EDITOR
EDataValidationResult UGFA_AddInputContextMapping::IsDataValid(FDataValidationContext& Context) const
{
    return EDataValidationResult();
}
#endif

void UGFA_AddInputContextMapping::OnGameFeatureRegistering()
{
    Super::OnGameFeatureRegistering();

    RegisterInputMappingContexts();
}

void UGFA_AddInputContextMapping::OnGameFeatureUnregistering()
{
    Super::OnGameFeatureUnregistering();

    UnregisterInputMappingContexts();
}

void UGFA_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
    auto& ActiveData = ContextData.FindOrAdd(Context);
    if (!ensure(ActiveData.ExtRequestHandles.IsEmpty()) ||
        !ensure(ActiveData.ControllersAddedTo.IsEmpty()))
    {
        Reset(ActiveData);
    }

    Super::OnGameFeatureActivating(Context);
}

void UGFA_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    Super::OnGameFeatureDeactivating(Context);

    auto ActiveData = ContextData.Find(Context);
    if (ensure(ActiveData))
    {
        Reset(*ActiveData);
    }
}

void UGFA_AddInputContextMapping::RegisterInputMappingContexts()
{
}

void UGFA_AddInputContextMapping::UnregisterInputMappingContexts()
{
}

void UGFA_AddInputContextMapping::RegisterInputMappingContexts(UGameInstance* GameInstance)
{
}

void UGFA_AddInputContextMapping::UnregisterInputMappingContexts(UGameInstance* GameInstance)
{
}

void UGFA_AddInputContextMapping::RegisterInputMappingContexts(ULocalPlayer* LocalPlayer)
{
}

void UGFA_AddInputContextMapping::UnregisterInputMappingContexts(ULocalPlayer* LocalPlayer)
{
}

void UGFA_AddInputContextMapping::Reset(FPerContextData& ActiveData)
{
}

void UGFA_AddInputContextMapping::AddInputMapping(UPlayer* Player, FPerContextData& ActiveData)
{
}

void UGFA_AddInputContextMapping::RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData)
{
}

void UGFA_AddInputContextMapping::HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
}
