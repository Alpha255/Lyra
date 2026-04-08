// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "GameUIManagerSubsystem.generated.h"

UCLASS(Abstract, config = Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    const class UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
    class UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

protected:
    void SetUIPolicy(class UGameUIPolicy* Policy);
private:
    UPROPERTY(Transient)
    TObjectPtr<class UGameUIPolicy> CurrentPolicy = nullptr;

    UPROPERTY(Config, EditAnywhere)
    TSoftClassPtr<class UGameUIPolicy> DefaultPolicyClass;
};