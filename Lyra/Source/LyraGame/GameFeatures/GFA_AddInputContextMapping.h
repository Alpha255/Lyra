// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_Base.h"
#include "GFA_AddInputContextMapping.generated.h"

/**
 * 
 */

USTRUCT()
struct FLyraInputMappingContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input", Meta = (AssetBundles = "Client,Server"))
    TSoftObjectPtr<class UInputMappingContext> InputMapping;

    UPROPERTY(EditAnywhere, Category = "Input")
    int32 Priority = 0;

    UPROPERTY(EditAnywhere, Category = "Input")
    bool bRegisterWithSettings = true;
};

UCLASS(MinimalAPI, Meta = (DisplayName = "Add Input Mapping"))
class UGFA_AddInputContextMapping : public UGameFeatureAction_Base
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

    virtual void OnGameFeatureRegistering() override;
    virtual void OnGameFeatureUnregistering() override;
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

    virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

    UPROPERTY(EditAnywhere, Category = "Input")
    TArray<FLyraInputMappingContext> InputMappings;

private:
    struct FPerContextData
    {
        TArray<TSharedPtr<struct FComponentRequestHandle>> ExtRequestHandles;
        TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
    };

    void RegisterInputMappingContexts();
    void UnregisterInputMappingContexts();

    void RegisterInputMappingContexts(UGameInstance* GameInstance);
    void UnregisterInputMappingContexts(UGameInstance* GameInstance);

    void RegisterInputMappingContexts(ULocalPlayer* LocalPlayer);
    void UnregisterInputMappingContexts(ULocalPlayer* LocalPlayer);

    void Reset(FPerContextData& ActiveData);

    void AddInputMapping(UPlayer* Player, FPerContextData& ActiveData);
    void RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData);

    void HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

    TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

    FDelegateHandle RegisterIM_Deletage;
};
