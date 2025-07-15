// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/AsyncAction_ExperienceReady.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "TimerManager.h"

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
    UAsyncAction_ExperienceReady* Action = nullptr;

    if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        Action = NewObject<UAsyncAction_ExperienceReady>();
        Action->WorldPtr = World;
        Action->RegisterWithGameInstance(World);
    }

    return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
    if (auto World = WorldPtr.Get())
    {
        if (auto GameState = World->GetGameState())
        {
            OnExperienceLoading(GameState);
        }
        else
        {
            World->GameStateSetEvent.AddUObject(this, &ThisClass::OnGameStateSet);
        }
    }
    else
    {
        SetReadyToDestroy();
    }
}

void UAsyncAction_ExperienceReady::OnGameStateSet(AGameStateBase* GameState)
{
    if (auto World = WorldPtr.Get())
    {
        World->GameStateSetEvent.RemoveAll(this);
    }

    OnExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::OnExperienceLoading(AGameStateBase* GameState)
{
    check(GameState);

    auto ExperienceComp = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
    check(ExperienceComp);

    if (ExperienceComp->IsExperienceLoaded())
    {
        auto World = GameState->GetWorld();
        check(World);

        World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::OnExperienceReady));
    }
    else
    {
        ExperienceComp->OnExperienceLoaded(FLyraOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded), ELyraExperienceLoadPriority::Normal);
    }
}

void UAsyncAction_ExperienceReady::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
    OnExperienceReady();
}

void UAsyncAction_ExperienceReady::OnExperienceReady()
{
    OnExperienceReadyDelegate.Broadcast();
    SetReadyToDestroy();
}
