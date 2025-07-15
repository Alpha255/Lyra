// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExperienceReady);


UCLASS()
class LYRAGAME_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
    static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

    virtual void Activate() override;

    UPROPERTY(BlueprintAssignable)
    FOnExperienceReady OnExperienceReadyDelegate;

private:
    void OnGameStateSet(AGameStateBase* GameState);
    void OnExperienceLoading(AGameStateBase* GameState);
    void OnExperienceLoaded(const class ULyraExperienceDefinition* Experience);
    void OnExperienceReady();

    TWeakObjectPtr<UWorld> WorldPtr;
};
