// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "LyraPlayerStart.generated.h"

/**
 * 
 */

enum class ELyraPlayerStartOccupancyState
{
	Empty,
	Partial,
	Full
};

UCLASS()
class LYRAGAME_API ALyraPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	ELyraPlayerStartOccupancyState GetOccupancyState(AController* Controller) const;

	bool IsOccupied() const;

	bool TryOccupy(AController* Controller);

protected:
	void CheckUnOccupied();

	UPROPERTY(Transient)
	TObjectPtr<AController> OccupiedBy = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Player Start Occupy")
	float ExpirationCheckInterval = 1.0f;

	FTimerHandle ExpirationCheckTimer;
};
