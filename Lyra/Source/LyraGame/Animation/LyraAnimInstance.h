// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "LyraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	ULyraAnimInstance(const FObjectInitializer& ObjectInitializer);

protected:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

protected:

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
