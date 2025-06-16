// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraExperienceManagerComponent.h"
#include "System/LyraAssetManager.h"
#include "LyraGame/LyraLogChannel.h"
#include "LyraExperienceDefinition.h"
#include "Net/UnrealNetwork.h"

ULyraExperienceManagerComponent::ULyraExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void ULyraExperienceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentExperience);
}

void ULyraExperienceManagerComponent::SetExperience(FPrimaryAssetId AssetId)
{
	auto AssetPath = ULyraAssetManager::Get().GetPrimaryAssetPath(AssetId);
	auto AssetClass = Cast<UClass>(AssetPath.TryLoad());

	check(AssetClass);
	CurrentExperience = GetDefault<ULyraExperienceDefinition>(AssetClass);
	check(CurrentExperience);

	StartLoadExperience(); // On server
}

void ULyraExperienceManagerComponent::OnRep_Experience()
{
	StartLoadExperience(); // On client
}

void ULyraExperienceManagerComponent::StartLoadExperience()
{
	check(CurrentExperience && ExperienceLoadState == ELyraExperienceLoadState::Unloaded);

	UE_LOG(LogLyra, Log, TEXT("Start load experience data: %s, %s"), *CurrentExperience->GetPrimaryAssetId().ToString(), *GetClientServerContextString());
}
