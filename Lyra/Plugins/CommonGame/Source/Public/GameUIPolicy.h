// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "GameUIPolicy.generated.h"

USTRUCT()
struct FRootViewportLayoutInfo
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    TObjectPtr<class ULocalPlayer> LocalPlayer = nullptr;

    UPROPERTY(Transient)
    TObjectPtr<class UCommonHUDLayout> RootLayout = nullptr;

    UPROPERTY(Transient)
    bool bAddedToViewport = false;

    FRootViewportLayoutInfo() = default;

    FRootViewportLayoutInfo(class ULocalPlayer* InLocalPlayer, class UCommonHUDLayout* InLayout, bool bIsInViewport)
        : LocalPlayer(InLocalPlayer)
        , RootLayout(InLayout)
        , bAddedToViewport(bIsInViewport)
    {
    }

    bool operator==(const class ULocalPlayer* OtherPlayer) { return LocalPlayer == OtherPlayer; }
};

UCLASS(Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class COMMONGAME_API UGameUIPolicy : public UObject
{
    GENERATED_BODY()

public:
    class UCommonHUDLayout* GetRootLayout(const class UCommonLocalPlayer* LocalPlayer) const;

    virtual UWorld* GetWorld() const override;
    class UGameUIManagerSubsystem* GetOwningUIManager() const;
protected:
    void AddLayoutToViewport(class UCommonLocalPlayer* LocalPlayer, class UCommonHUDLayout* Layout);
    void RemoveLayoutFromViewport(class UCommonLocalPlayer* LocalPlayer, class UCommonHUDLayout* Layout);

    void CreateLayoutWidget(class UCommonLocalPlayer* LocalPlayer);
    TSubclassOf<class UCommonHUDLayout> GetLayoutWidgetClass(class UCommonLocalPlayer* LocalPlayer);
private:
    friend class UGameUIManagerSubsystem;

    void OnAddLocalPlayer(UCommonLocalPlayer* NewPlayer);
    void OnRemoveLocalPlayer(UCommonLocalPlayer* Player);
    void OnDestroyLocalPlayer(UCommonLocalPlayer* Player);

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<class UCommonHUDLayout> LayoutClass;

    UPROPERTY(Transient)
    TArray<FRootViewportLayoutInfo> RootViewportLayouts;
};