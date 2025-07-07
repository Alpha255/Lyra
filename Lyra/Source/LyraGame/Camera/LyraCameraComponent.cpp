// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCameraComponent.h"
#include "Camera/LyraCameraModeStack.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "LyraCameraMode.h"

ULyraCameraComponent::ULyraCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
	, FieldOfViewOffset(0.0f)
{
}

void ULyraCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<ULyraCameraModeStack>(this);
		check(CameraModeStack);
	}
}

void ULyraCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FLyraCameraModeView CameraModeView;
	CameraModeStack->Evaluate(DeltaTime, CameraModeView);

	if (auto Pawn = Cast<APawn>(GetTargetActor()))
	{
		if (auto PlayerController = Pawn->GetController<APlayerController>())
		{
			PlayerController->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;

	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void ULyraCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (CameraModeStack->IsActive() && DetermineCameModeDelegate.IsBound())
	{
		if (auto CameraMode = DetermineCameModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}
