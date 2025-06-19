// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraExperienceManagerComponent.h"
#include "System/LyraAssetManager.h"
#include "LyraGame/LyraLogChannel.h"
#include "LyraExperienceDefinition.h"
#include "Net/UnrealNetwork.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"

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

	UE_LOG(LogLyra, Log, TEXT("Start load experience data: %s, %s"), *CurrentExperience->GetPrimaryAssetId().ToString(), 
		*GetClientServerContextString(this));

	ExperienceLoadState = ELyraExperienceLoadState::Loading;

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	BundlesToLoad.Add(TEXT("Equipped"));

	const ENetMode NetMode = GetOwner()->GetNetMode();
	const bool bLoadOnClient = GIsEditor || NetMode != NM_DedicatedServer;
	const bool bLoadOnServer = GIsEditor || NetMode != NM_Client;
	if (bLoadOnClient)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadOnServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	ULyraAssetManager& AssetMgr = ULyraAssetManager::Get();
	auto BundleLoadHandle = AssetMgr.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	FStreamableDelegate AssetListLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded);

	if (BundleLoadHandle && BundleLoadHandle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(AssetListLoadedDelegate);
	}
	else
	{
		BundleLoadHandle->BindCompleteDelegate(AssetListLoadedDelegate);

		BundleLoadHandle->BindCancelDelegate(FStreamableDelegate::CreateLambda([AssetListLoadedDelegate]() {
			AssetListLoadedDelegate.ExecuteIfBound();
		}));
	}
}

void ULyraExperienceManagerComponent::ApplyGameplayActions()
{
	check(ExperienceLoadState != ELyraExperienceLoadState::Loaded);

	ExperienceLoadState = ELyraExperienceLoadState::ExecutingActions;

	FGameFeatureActivatingContext Context;
	if (auto WorldContext = GEngine->GetWorldContextFromWorld(GetWorld()))
	{
		Context.SetRequiredWorldContextHandle(WorldContext->ContextHandle);
	}

	auto ActivateActionList = [&](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (auto Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating(Context);
				}
			}
		};

	ActivateActionList(CurrentExperience->GameFeatureActions);

	ExperienceLoadState = ELyraExperienceLoadState::Loaded;

	for (auto Delegate : TArray<FOnLyraExperienceLoaded*>{ 
		&OnExperienceLoaded_HighPriority, 
		&OnExperienceLoaded_NormalPriority, 
		&OnExperienceLoaded_LowPriority })
	{
		Delegate->Broadcast(CurrentExperience);
		Delegate->Clear();
	}
}

void ULyraExperienceManagerComponent::OnExperienceLoaded()
{
	check(ExperienceLoadState == ELyraExperienceLoadState::Loading && CurrentExperience);

	UE_LOG(LogLyra, Log, TEXT("Load experience data completed: %s, %s"), *CurrentExperience->GetPrimaryAssetId().ToString(),
		*GetClientServerContextString(this));

	ApplyGameplayActions();
}