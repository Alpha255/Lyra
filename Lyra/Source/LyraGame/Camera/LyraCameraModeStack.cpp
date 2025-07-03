// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCameraModeStack.h"
#include "Camera/LyraCameraMode.h"

void ULyraCameraModeStack::Activate()
{

}

void ULyraCameraModeStack::Deactivate()
{

}

void ULyraCameraModeStack::PushCameraMode(TSubclassOf<ULyraCameraMode> CameraMode)
{
}

bool ULyraCameraModeStack::Evaluate(float DeltaTime, FLyraCameraModeView& OutCameraModeView)
{
    return false;
}
