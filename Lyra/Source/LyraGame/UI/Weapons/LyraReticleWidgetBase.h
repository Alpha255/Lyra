// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "LyraReticleWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LYRAGAME_API ULyraReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintImplementableEvent)
    void OnWeaponInitialize();

    UFUNCTION(BlueprintCallable)
    void InitializeFromWeapon(class ULyraWeaponInstance* Weapon);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float ComputeSpreadAngle() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float ComputeMaxSpreadRadiusScreenspace() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool HasFirstShotAccuracy() const;
protected:
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class ULyraWeaponInstance> WeaponInstance;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class ULyraInventoryItemInstance> InventoryItemInstance;
};
