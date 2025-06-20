// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LyraCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	ULyraCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
};
