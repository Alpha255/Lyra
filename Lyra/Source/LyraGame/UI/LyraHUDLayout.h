// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LyraActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "LyraHUDLayout.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Lyra HUD Layout", Category = "Lyra|HUD"))
class LYRAGAME_API ULyraHUDLayout : public ULyraActivatableWidget
{
	GENERATED_BODY()
	
public:
    ULyraHUDLayout(const FObjectInitializer& ObjectInitializer);

    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;

protected:
    void OnEscapeAction();

    void OnInputDeviceConnectionChanged(EInputDeviceConnectionState ConnectionState, FPlatformUserId UserId, FInputDeviceId InputDeviceId);

    void OnInputDevicePairingChanged(FInputDeviceId InputDeviceId, FPlatformUserId NewUserId, FPlatformUserId OldUserId);

    void NotifyControllerStateChangeForDisconectScreen();

    virtual void ProcessControllerDevicesChangedForDisconnectScreen();

    virtual bool ShouldDisplayControllerDisconnectScreen() const;

    UFUNCTION(BlueprintNativeEvent, Category = "Controller Disconnect Menu")
    void ShowControllerDisconnectMenu();

    UFUNCTION(BlueprintNativeEvent, Category = "Controller Disconnect Menu")
    void HideControllerDisconnectMenu();

    UPROPERTY(EditDefaultsOnly)
    TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;

    //UPROPERTY(EditDefaultsOnly, Category = "Controller Disconnect Menu")
    //TSubclassOf<class ULyraControllerDisconnectedScreen> ControllerDisconnectedScreen;

    UPROPERTY(EditDefaultsOnly, Category = "Controller Disconnect Menu")
    FGameplayTagContainer PlatformControllerDisconnectTag;
    
    UPROPERTY(Transient)
    TObjectPtr<UCommonActivatableWidget> SpawndControllerDisconnectScreen;

    FTSTicker::FDelegateHandle RequestProcessControllerStateDelegate;
};
