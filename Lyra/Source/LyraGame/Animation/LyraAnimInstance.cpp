// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/LyraAnimInstance.h"

ULyraAnimInstance::ULyraAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
EDataValidationResult ULyraAnimInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	GameplayTagPropertyMap.IsDataValid(this, ValidationErrors);

	return ((ValidationErrors.Num() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif