// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFeatures/LyraGameFeatureAction_AddInputContextMapping.h"
#include "LyraHeroComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	ULyraHeroComponent(const FObjectInitializer& ObjectInitializerx);

	virtual FName GetFeatureName() const override { return NAME_Feature; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

    bool IsReadyToBindInputs() const { return bReadyToBindInputs; }

    UFUNCTION(BlueprintPure, Category = "Lyra|Hero")
    static ULyraHeroComponent* FindHeroComponent(const AActor* Actor) { return Actor ? Actor->FindComponentByClass<ULyraHeroComponent>() : nullptr; }

    void SetAbilityCameraMode(TSubclassOf<class ULyraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& Handle);
    void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& Handle);

	static const FName NAME_Feature;
    static const FName NAME_ReadyToBindInputs;
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

    void Input_AbilityTagPressed(FGameplayTag Tag);
    void Input_AbilityTagReleased(FGameplayTag Tag);

    void Input_Move(const struct FInputActionValue& ActionValue);
    void Input_LookMouse(const struct FInputActionValue& ActionValue);
    void Input_LookStick(const struct FInputActionValue& ActionValue);
    void Input_Crouch(const struct FInputActionValue& ActionValue);
    void Input_AutoRun(const struct FInputActionValue& ActionValue);

    UPROPERTY(EditAnywhere)
    TArray<FLyraInputMappingContext> DefaultInputMappingContexts;

    UPROPERTY()
    TSubclassOf<class ULyraCameraMode> AbilityCameraMode;

    FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;

    bool bReadyToBindInputs = false;
};
