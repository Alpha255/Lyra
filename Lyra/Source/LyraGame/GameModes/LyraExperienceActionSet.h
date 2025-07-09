// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceActionSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, NotBlueprintable)
class LYRAGAME_API ULyraExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	UPROPERTY(EditAnywhere, Instanced, Category = "Actions")
	TArray<TObjectPtr<class UGameFeatureAction>> Actions;

	UPROPERTY(EditAnywhere, Category = "Dependencies")
	TArray<FString> GameFeaturesToEnable;
};
