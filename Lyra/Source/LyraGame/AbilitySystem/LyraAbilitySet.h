// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraAbilitySet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LYRAGAME_API ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    ULyraAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GiveToAbilitySystem(class ULyraAbilitySystemComponent* LyraASC, struct FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const;
protected:

};
