// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LyraInputConfig.h"

ULyraInputConfig::ULyraInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* ULyraInputConfig::FindNativeInputActionForTag(const FGameplayTag& GameplayTag, bool bLogIfNotFound) const
{
    for (auto& Action : NativeInputActions)
    {
        if (Action.InputAction && Action.InputTag == GameplayTag)
        {
            return Action.InputAction;
        }
    }

    if (bLogIfNotFound)
    {

    }

	return nullptr;
}
