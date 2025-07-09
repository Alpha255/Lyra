// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraExperienceActionSet.h"
#include "GameFeatureAction.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "Lyra"

EDataValidationResult ULyraExperienceActionSet::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    for (int32 Index = 0; Index < Actions.Num(); ++Index)
    {
        if (Actions[Index])
        {
            Result = CombineDataValidationResults(Result, Actions[Index]->IsDataValid(Context));
        }
        else
        {
            Result = EDataValidationResult::Invalid;
            Context.AddError(FText::Format(LOCTEXT("ActionEntryIsNull", "Null entry at index {0} in actions"), FText::AsNumber(Index)));
        }
    }

    return Result;
}

#if WITH_EDITORONLY_DATA
void ULyraExperienceActionSet::UpdateAssetBundleData()
{
    Super::UpdateAssetBundleData();

    for (auto Action : Actions)
    {
        if (Action)
        {
            Action->AddAdditionalAssetBundleData(AssetBundleData);
        }
    }
}
#endif
