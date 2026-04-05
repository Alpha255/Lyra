// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LyraHUD.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

ALyraHUD::ALyraHUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void ALyraHUD::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ALyraHUD::BeginPlay()
{
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

    Super::BeginPlay();
}

void ALyraHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

    Super::EndPlay(EndPlayReason);
}

void ALyraHUD::GetDebugActorList(TArray<AActor*>&InOutList)
{
    Super::GetDebugActorList(InOutList);

    for (TObjectIterator<UAbilitySystemComponent> It; It; ++It)
    {
        if (auto AbilitySysComp = *It)
        {
            if (!AbilitySysComp->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
            {
                auto AvatarActor = AbilitySysComp->GetAvatarActor();
                auto OwnerActor = AbilitySysComp->GetOwnerActor();

                if (AvatarActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor))
                {
                    AddActorToDebugList(AvatarActor, InOutList, GetWorld());
                }
                else if (OwnerActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
                {
                    AddActorToDebugList(OwnerActor, InOutList, GetWorld());
                }
            }
        }
    }
}
