// Copyright Epic Games, Inc. All Rights Reserved.


#include "Cosmetics/LyraCosmeticsAnimation.h"

TSubclassOf<class UAnimInstance> FLyraAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& Tags) const
{
    for (auto& Rule : LayerRules)
    {
        if (Rule.Layer && Tags.HasAll(Rule.RequiredTags))
        {
            return Rule.Layer;
        }
    }

    return DefaultLayer;
}

USkeletalMesh* FLyraAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& Tags) const
{
	for (auto& Rule : MeshRules)
	{
		if (Rule.Mesh != nullptr && Tags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}

	return DefaultMesh;
}
