// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraGameState.h"
#include "GameModes/LyraExperienceManagerComponent.h"

ALyraGameState::ALyraGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceMgrComp = CreateDefaultSubobject<ULyraExperienceManagerComponent>(TEXT("LyraExperienceManagerComponent"));
}
