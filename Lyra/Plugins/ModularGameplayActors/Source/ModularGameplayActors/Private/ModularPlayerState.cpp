// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"

void AModularPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void AModularPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

void AModularPlayerState::Reset()
{
	Super::Reset();

	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (auto Component : ModularComponents)
	{
		Component->Reset();
	}
}

void AModularPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	TInlineComponentArray<UPlayerStateComponent*> PlayerStateComponents;
	GetComponents(PlayerStateComponents);

	for (auto SrcComponent : PlayerStateComponents)
	{
		if (auto DstComponent = Cast<UPlayerStateComponent>(static_cast<UObject*>(FindObjectWithOuter(PlayerState, SrcComponent->GetClass(), SrcComponent->GetFName()))))
		{
			SrcComponent->CopyProperties(DstComponent);
		}
	}
}
