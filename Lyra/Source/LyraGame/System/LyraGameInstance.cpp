// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LyraGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraGameplayTags.h"

ULyraGameInstance::ULyraGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraGameInstance::Init()
{
	Super::Init();

	auto GameFrameworkCompMgr = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(GameFrameworkCompMgr))
	{
		GameFrameworkCompMgr->RegisterInitState(LyraGameplayTags::InitState_Spawned, false, FGameplayTag());
		GameFrameworkCompMgr->RegisterInitState(LyraGameplayTags::InitState_DataAvailable, false, LyraGameplayTags::InitState_Spawned);
		GameFrameworkCompMgr->RegisterInitState(LyraGameplayTags::InitState_DataInitialized, false, LyraGameplayTags::InitState_DataAvailable);
		GameFrameworkCompMgr->RegisterInitState(LyraGameplayTags::InitState_GameplayReady, false, LyraGameplayTags::InitState_DataInitialized);
	}
}
