// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPlayerController.h"
#include "CommonLocalPlayer.h"

ACommonPlayerController::ACommonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (auto LocalPlayer = Cast<UCommonLocalPlayer>(Player))
	{
		if (PlayerState)
		{
		}
	}
}

void ACommonPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void ACommonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACommonPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACommonPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
