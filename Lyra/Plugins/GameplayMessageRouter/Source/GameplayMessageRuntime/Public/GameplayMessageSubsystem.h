// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayMessageTypes.h"
#include "GameplayMessageSubsystem.generated.h"

UCLASS()
class GAMEPLAYMESSAGERUNTIME_API UGameplayMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UGameplayMessageSubsystem& Get(const UObject* WorldContextObject);

	template<class FMessageType>
	void BroadcastMessage(FGameplayTag Channel, const FMessageType& Message)
	{
		const UScriptStruct* MessageType = TBaseStructure<FMessageType>::Get();
		BroadcastMessageInternal(Channel, MessageType, &Message);
	}

private:
	void BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* MessageType, const void* MessageData);
};
