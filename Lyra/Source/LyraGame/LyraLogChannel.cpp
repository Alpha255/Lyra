// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraLogChannel.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogLyra);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (auto Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (auto Comp = Cast<UActorComponent>(ContextObject))
	{
		Role = Comp->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return Role == ROLE_Authority ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("None");

}
