// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayMessageSubsystem.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, GameplayMessageRuntime);

UGameplayMessageSubsystem& UGameplayMessageSubsystem::Get(const UObject* WorldContextObject)
{
	auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);

	auto Subsystem = UGameInstance::GetSubsystem<UGameplayMessageSubsystem>(World->GetGameInstance());
	check(Subsystem);

	return *Subsystem;
}

void UGameplayMessageSubsystem::BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* MessageType, const void* MessageData)
{
	for (FGameplayTag Tag = Channel; Tag.IsValid(); Tag.RequestDirectParent())
	{

	}
}
