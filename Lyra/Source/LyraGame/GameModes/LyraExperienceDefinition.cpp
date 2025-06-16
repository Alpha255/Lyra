// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraExperienceDefinition.h"
#include "GameFeatureAction.h"

EDataValidationResult ULyraExperienceDefinition::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	return Result;
}
