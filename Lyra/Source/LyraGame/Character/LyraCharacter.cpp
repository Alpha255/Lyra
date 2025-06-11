// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCharacter.h"
#include "LyraCharacterMovementComponent.h"
#include "LyraPawnComponent.h"
#include "Camera/LyraCameraComponent.h"

ALyraCharacter::ALyraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PawnComponent = CreateDefaultSubobject<ULyraPawnComponent>(TEXT("PawnComponent"));

	CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>(TEXT("CameraComponent"));
}
