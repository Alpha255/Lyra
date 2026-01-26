// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraAbilitySet.h"
#include "LyraAbilitySet.h"

ULyraAbilitySet::ULyraAbilitySet(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULyraAbilitySet::GiveToAbilitySystem(ULyraAbilitySystemComponent* LyraASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
}
