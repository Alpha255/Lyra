// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "LyraTaggedActor.generated.h"

UCLASS()
class LYRAGAME_API ALyraTaggedActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor")
	FGameplayTagContainer StaticGameplayTags;
};
