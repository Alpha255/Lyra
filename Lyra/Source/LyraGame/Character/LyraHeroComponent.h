// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
//#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"
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

	static const FName NAME_Feature;
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

    //TArray<FInputMappingContextAndPriority> DefaultInputMappings;
};
