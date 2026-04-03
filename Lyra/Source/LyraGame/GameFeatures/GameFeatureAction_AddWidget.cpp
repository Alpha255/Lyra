// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddWidget.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/GameInstance.h"
#include "Components/GameFrameworkComponentManager.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "LyraGameFeatures"

void UGameFeatureAction_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    Super::OnGameFeatureDeactivating(Context);

    if (FPerContextData* ActiveData = ContextData.Find(Context))
    {
        Reset(*ActiveData);
    }
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddWidget::AddAdditionalAssetBundleData(FAssetBundleData & AssetBundleData)
{
    for (const auto& Entry : Widgets)
    {
        AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.WidgetClass.ToSoftObjectPath().GetAssetPath());
    }
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddWidget::IsDataValid(FDataValidationContext & Context) const
{
    EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    for (int32 Index = 0; Index < Layout.Num(); ++Index)
    {
        auto& Entry = Layout[Index];

        if (!Entry.LayoutClass.IsValid())
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("LayoutHasNullClass", "Null WidgetClass at index {0} in Layout"), FText::AsNumber(Index)));
        }

        if (!Entry.LayerID.IsValid())
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("LayoutHasNoTag", "LayerID is not set at index {0} in Widgets"), FText::AsNumber(Index)));
        }
    }

    for (int32 Index = 0; Index < Widgets.Num(); ++Index)
    {
        auto& Entry = Widgets[Index];

        if (!Entry.WidgetClass.IsValid())
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("EntryHasNullClass", "Null WidgetClass at index {0} in Widgets"), FText::AsNumber(Index)));
        }

        if (!Entry.SlotID.IsValid())
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("EntryHasNoTag", "SlotID is not set at index {0} in Widgets"), FText::AsNumber(Index)));
        }
    }

    return EDataValidationResult();
}
#endif

void UGameFeatureAction_AddWidget::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
    auto World = WorldContext.World();
    auto GameInstance = WorldContext.OwningGameInstance;
    auto& ActiveData = ContextData.FindOrAdd(ChangeContext);

    if (GameInstance && World && World->IsGameWorld())
    {
        if (auto CompMgr = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
        {

        }
    }
}

void UGameFeatureAction_AddWidget::Reset(FPerContextData& ActiveData)
{
    ActiveData.ComponentRequests.Empty();

    for (auto& [Key, Data] : ActiveData.ActorData)
    {
    }

    ActiveData.ActorData.Empty();
}

void UGameFeatureAction_AddWidget::HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext& ChangeContext)
{
    FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
    if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
    {
        RemoveWidgets(Actor, ActiveData);
    }
    else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
    {
        AddWidgets(Actor, ActiveData);
    }
}

void UGameFeatureAction_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
}

void UGameFeatureAction_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
}

#undef LOCTEXT_NAMESPACE
