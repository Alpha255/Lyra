// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameInstance.h"
#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"

UCommonGameInstance::UCommonGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
    int32 Ret = Super::AddLocalPlayer(NewPlayer, UserId);
    if (Ret != INDEX_NONE)
    {
        if (!PrimaryPlayer.IsValid())
        {
            PrimaryPlayer = NewPlayer;
        }

        GetSubsystem<UGameUIManagerSubsystem>()->OnAddLocalPlayer(Cast<UCommonLocalPlayer>(NewPlayer));
    }

    return Ret;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
    if (PrimaryPlayer == ExistingPlayer)
    {
        PrimaryPlayer.Reset();
    }
    
    GetSubsystem<UGameUIManagerSubsystem>()->OnDestroyLocalPlayer(Cast<UCommonLocalPlayer>(ExistingPlayer));

    return Super::RemoveLocalPlayer(ExistingPlayer);
}
