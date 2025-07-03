// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraPawnData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraPawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lyra|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lyra|Input")
	TObjectPtr<class ULyraInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lyra|Camera")
	TSubclassOf<class ULyraCameraMode> DefaultCameraMode;
};
