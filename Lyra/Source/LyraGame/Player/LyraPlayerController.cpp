// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerController.h"
#include "LyraPlayerState.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ALyraPlayerState* ALyraPlayerController::GetLyraPlayerState() const
{
    return CastChecked<ALyraPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULyraAbilitySystemComponent* ALyraPlayerController::GetLyraAbilitySystemComponent() const
{
    if (auto LyraPlayerState = GetLyraPlayerState())
    {
        return LyraPlayerState->GetLyraAbilitySystemComponent();
    }

    return nullptr;
}

void ALyraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void ALyraPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ALyraPlayerController::OnCameraPenetratingTarget()
{
	bHideViewTargetPawnNextFrame = true;
}

void ALyraPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    if (auto AbilitySysComp = GetLyraAbilitySystemComponent())
    {
        AbilitySysComp->ProcessAbilityInput(DeltaTime, bGamePaused);
    }

    Super::PostProcessInput(DeltaTime, bGamePaused);
}
