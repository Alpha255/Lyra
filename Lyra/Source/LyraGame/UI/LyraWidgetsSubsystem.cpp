// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LyraWidgetsSubsystem.h"

ULyraWidgetsSubsystem::ULyraWidgetsSubsystem()
{
}

void ULyraWidgetsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (auto DefaultObeject = GetDefault<ULyraWidgetsSubsystem>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Class Path: %s"), *DefaultObeject->GetClass()->GetPathName());
        UE_LOG(LogTemp, Warning, TEXT("Config Name: %s"), *DefaultObeject->GetClass()->GetConfigName());
    }
}

void ULyraWidgetsSubsystem::Deinitialize()
{
    Super::Deinitialize();
}
