// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LyraHUD.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class LYRAGAME_API ALyraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    ALyraHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void PreInitializeComponents() override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;
};
