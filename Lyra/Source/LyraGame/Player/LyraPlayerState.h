// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "LyraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraPlayerState : public AModularPlayerState
{
	GENERATED_BODY()
	
public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;

	const class ULyraPawnData* GetPawnData() const { return PawnData; }

	void SetPawnData(const class ULyraPawnData* InPawnData);
protected:
	UFUNCTION()
	void OnRep_PawnData();

	UPROPERTY(ReplicatedUsing=OnRep_PawnData)
	TObjectPtr<const class ULyraPawnData> PawnData;

private:
};
