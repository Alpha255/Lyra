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

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	TObjectPtr<const class ULyraPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<class UGameFeatureAction>> GameFeatureActions;
};
