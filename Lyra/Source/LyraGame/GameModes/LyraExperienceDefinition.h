// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceDefinition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TObjectPtr<const class ULyraPawnData> DefaultPawnData;
};
