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
    void BindAbilityAction(const ULyraInputConfig* InputConfig, TObject* Object, TPressedFunc PressedFunc, TReleasedFunc ReleasedFunc, TArray<uint32>& BindHandles)
    {
        check(InputConfig);

        for (auto& const InputAction : InputConfig->AbilityInputActions)
        {
            if (InputAction.InputAction && InputAction.InputTag.IsValid())
            {
                if (PressedFunc)
                {
                    BindHandles.Add(BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, InputAction.InputTag).GetHandle());
                }
                if (ReleasedFunc)
                {
                    BindHandles.Add(BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag).GetHandle());
                }
            }
        }
    }
};
