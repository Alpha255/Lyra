// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterialWithTags.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PhysicalProperties")
	FGameplayTagContainer Tags;
};
