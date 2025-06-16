// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraTaggedActor.h"

void ALyraTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}
