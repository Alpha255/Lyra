// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */

namespace LyraGameplayTags
{
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LookMouse);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LookStick);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	LYRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);
}
