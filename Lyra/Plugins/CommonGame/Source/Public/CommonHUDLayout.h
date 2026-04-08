// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonHUDLayout.generated.h"

UCLASS(Abstract, meta = (DisableNativeTick))
class COMMONGAME_API UCommonHUDLayout : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    UCommonHUDLayout(const FObjectInitializer& ObjectInitializer);

    template<class TActivatalbeWidget = UCommonActivatableWidget>
    TActivatalbeWidget* AddWidgetToLayerStack(FGameplayTag LayerTag, UClass* WidgetClass)
    {
        return AddWidgetToLayerStack<TActivatalbeWidget>(LayerTag, WidgetClass, [](TActivatalbeWidget&){});
    }

    template<class TActivatalbeWidget = UCommonActivatableWidget>
    TActivatalbeWidget* AddWidgetToLayerStack(FGameplayTag LayerTag, UClass* WidgetClass, TFunctionRef<void(TActivatalbeWidget&)> InitInstanceFunc)
    {
        static_assert(TIsDerivedFrom<TActivatalbeWidget, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets valid");

        if (auto Layer = GetLayerWidget(LayerTag))
        {
            return Layer->AddWidget<TActivatalbeWidget>(WidgetClass, InitInstanceFunc);
        }

        return nullptr;
    }

    UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerTag);
protected:
    UFUNCTION(BlueprintCallable, Category = "Layer")
    void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, 
        UCommonActivatableWidgetContainerBase* LayerWidget);
private:
    UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
