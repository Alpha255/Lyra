// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LyraHUDLayout.h"

#include "CommonUISettings.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonUIModule.h"
#include "NativeGameplayTags.h"
#include "UI/LyraActivatableWidget.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_Input_PrimarlyController, "Platform.Trait.Input.PrimarlyController");

ULyraHUDLayout::ULyraHUDLayout(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , SpawndControllerDisconnectScreen(nullptr)
{
    PlatformControllerDisconnectTag.AddTag(TAG_Platform_Trait_Input_PrimarlyController);
}

void ULyraHUDLayout::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, 
        FSimpleDelegate::CreateUObject(this, &ThisClass::OnEscapeAction)));

    if (ShouldDisplayControllerDisconnectScreen())
    {
        auto& DeviceMgr = IPlatformInputDeviceMapper::Get();
        DeviceMgr.GetOnInputDeviceConnectionChange().AddUObject(this, &ThisClass::OnInputDeviceConnectionChanged);
        DeviceMgr.GetOnInputDevicePairingChange().AddUObject(this, &ThisClass::OnInputDevicePairingChanged);
    }
}

void ULyraHUDLayout::NativeDestruct()
{
    Super::NativeDestruct();

    auto& DeviceMgr = IPlatformInputDeviceMapper::Get();
    DeviceMgr.GetOnInputDeviceConnectionChange().RemoveAll(this);
    DeviceMgr.GetOnInputDevicePairingChange().RemoveAll(this);

    if (RequestProcessControllerStateDelegate.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(RequestProcessControllerStateDelegate);
        RequestProcessControllerStateDelegate.Reset();
    }
}

void ULyraHUDLayout::OnEscapeAction()
{}

void ULyraHUDLayout::OnInputDeviceConnectionChanged(EInputDeviceConnectionState ConnectionState, FPlatformUserId UserId, FInputDeviceId InputDeviceId)
{}

void ULyraHUDLayout::OnInputDevicePairingChanged(FInputDeviceId InputDeviceId, FPlatformUserId NewUserId, FPlatformUserId OldUserId)
{}

void ULyraHUDLayout::NotifyControllerStateChangeForDisconectScreen()
{}

void ULyraHUDLayout::ProcessControllerDevicesChangedForDisconnectScreen()
{}

bool ULyraHUDLayout::ShouldDisplayControllerDisconnectScreen() const
{
    return false;
}

void ULyraHUDLayout::ShowControllerDisconnectMenu_Implementation()
{}

void ULyraHUDLayout::HideControllerDisconnectMenu_Implementation()
{}
