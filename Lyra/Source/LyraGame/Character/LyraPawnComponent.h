// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraPawnComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraPawnComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	ULyraPawnComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

	virtual FName GetFeatureName() const override { return NAME_Feature; }

	UFUNCTION(Blueprintpure, Category="Lyra|Pawn")
	static ULyraPawnComponent* GetPawnComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<ULyraPawnComponent>() : nullptr;
	}

	const class ULyraPawnData* GetPawnData() const { return PawnData; }
	void SetPawnData(const class ULyraPawnData* InPawnData);

	void OnControllerChanged();
	void OnPlayerStateReplicated();
	void OnSetupPlayerInputComponent();

	static const FName NAME_Feature;
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnRep_PawnData();

	UPROPERTY(EditInstanceOnly, ReplicatedUsing=OnRep_PawnData, Category="Lyra|Pawn")
	TObjectPtr<const class ULyraPawnData> PawnData;
};
