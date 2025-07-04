// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCameraModeStack.h"
#include "Camera/LyraCameraMode.h"

void ULyraCameraModeStack::Activate()
{
    if (!bIsActive)
    {
        bIsActive = true;

        for (auto CameraMode : CameraModes)
        {
            check(CameraMode);
            CameraMode->Activate();
        }
    }
}

void ULyraCameraModeStack::Deactivate()
{
    if (bIsActive)
    {
        bIsActive = false;

        for (auto CameraMode : CameraModes)
        {
            check(CameraMode);
            CameraMode->Deactivate();
        }
    }
}

void ULyraCameraModeStack::PushCameraMode(TSubclassOf<ULyraCameraMode> CameraModeClass)
{
    if (!CameraModeClass)
    {
        return;
    }

    auto CameraMode = GetCameraModeInstance(CameraModeClass);
    check(CameraMode);

    if (!CameraModes.IsEmpty() && CameraModes[0] == CameraMode)
    {
        return;
    }

    int32 CameraModeIndex = INDEX_NONE;
	int32 NumCameraModes = CameraModes.Num();
    float Weight = 1.0f;

    for (int32 Index = 0; Index < NumCameraModes; ++Index)
    {
        if (CameraModes[Index] == CameraMode)
        {
            CameraModeIndex = Index;
            Weight *= CameraMode->GetBlendWeight();
			break;
        }
        else
        {
			Weight = 1.0f - CameraModes[Index]->GetBlendWeight();
        }
    }

    if (CameraModeIndex == INDEX_NONE)
    {
        Weight = 0.0f;
    }
    else
    {
		CameraModes.RemoveAt(CameraModeIndex);
        --NumCameraModes;
    }

    const bool bShouldBlend = CameraMode->GetBlendTime() > 0.0f && NumCameraModes > 0;
    const float BlendWeight = bShouldBlend ? Weight : 1.0f;
    CameraMode->SetBlendWeight(BlendWeight);

    CameraModes.Insert(CameraMode, 0);
    CameraModes.Last()->SetBlendWeight(1.0f);

    if (CameraModeIndex == INDEX_NONE)
    {
        CameraMode->Activate();
    }
}

bool ULyraCameraModeStack::Evaluate(float DeltaTime, FLyraCameraModeView& OutCameraModeView)
{
    if (bIsActive)
    {
        Update(DeltaTime);
        Blend(OutCameraModeView);

        return true;
    }
    return false;
}

ULyraCameraMode* ULyraCameraModeStack::GetCameraModeInstance(TSubclassOf<ULyraCameraMode> CameraModeClass)
{
    if (CameraModeClass)
    {
        for (auto CameraMode : CameraModeInstances)
        {
            if (CameraMode && CameraMode->GetClass() == CameraModeClass)
            {
                return CameraMode;
            }
        }

        auto NewCameraMode = NewObject<ULyraCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
        check(NewCameraMode);

        CameraModeInstances.Add(NewCameraMode);

        return NewCameraMode;
    }
    return nullptr;
}

void ULyraCameraModeStack::Update(float DeltaTime)
{
    if (CameraModes.IsEmpty())
    {
        return;
    }

    int32 NumToRemove = 0;
    int32 StartIndex = INDEX_NONE;

    for (int32 Index = 0; Index < CameraModes.Num(); ++Index)
    {
        auto& CameraMode = CameraModes[Index];
        check(CameraMode);

        CameraMode->Update(DeltaTime);

        if (CameraMode->GetBlendWeight() >= 1.0f)
        {
            StartIndex = Index + 1;
            NumToRemove = CameraModes.Num() - StartIndex;
            break;
        }
    }

    if (NumToRemove > 0)
    {
        for (int32 Index = StartIndex; Index < CameraModes.Num(); ++Index)
        {
            auto CameraMode = CameraModes[Index];
            check(CameraMode);

            CameraMode->Deactivate();
        }

        CameraModes.RemoveAt(StartIndex, NumToRemove);
    }
}

void ULyraCameraModeStack::Blend(FLyraCameraModeView& OutCameraModeView)
{
    if (CameraModes.IsEmpty())
    {
        return;
    }

    auto& CameraMode = CameraModes.Last();
    check(CameraMode);

    OutCameraModeView = CameraMode->GetCameraModeView();

    for (int32 Index = CameraModes.Num() - 2; Index >= 0; --Index)
    {
        CameraMode = CameraModes[Index];
        check(CameraMode);

        OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
    }
}
