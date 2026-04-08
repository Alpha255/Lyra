// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"
#include "GameplayTagContainer.h"
#include "CommonUIBlueprintFunctionLibrary.generated.h"

UCLASS()
class COMMONGAME_API UCommonUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    static class UCommonActivatableWidget* PushWidgetToLayerStack(const class ULocalPlayer* LocalPlayer,
        UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag Layer,
        UPARAM(meta = (AllowAbstract = false)) TSubclassOf<class UCommonActivatableWidget> WidgetClass);
};
