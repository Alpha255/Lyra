// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/LyraInputConfig.h"
#include "LyraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
    template<class TObject, class TFunc>
    void BindNativeAction(const ULyraInputConfig* InputConfig, const FGameplayTag& GameplayTag, ETriggerEvent TriggerEvent, TObject* Object, TFunc Func, bool bLogIfNotFound)
    {
        check(InputConfig);

        if (auto InputAction = InputConfig->FindNativeInputActionForTag(GameplayTag, bLogIfNotFound))
        {
            BindAction(InputAction, TriggerEvent, Object, Func);
        }
    }

    template<class TObject, class TPressedFunc, class TReleasedFunc>
    void BindAbilityActions(const ULyraInputConfig* InputConfig, TObject* Object, TPressedFunc PressedFunc, TReleasedFunc ReleasedFunc, TArray<uint32>& BindHandles)
    {
        check(InputConfig);

        for (auto& Action : InputConfig->AbilityInputActions)
        {
            if (Action.InputAction && Action.InputTag.IsValid())
            {
                if (PressedFunc)
                {
                    BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
                }
                if (ReleasedFunc)
                {
                    BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
                }
            }
        }
    }
};
