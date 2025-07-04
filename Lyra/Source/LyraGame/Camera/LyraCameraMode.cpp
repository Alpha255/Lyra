// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCameraMode.h"
#include "Camera/LyraPlayerCameraManager.h"
#include "Camera/LyraCameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

FLyraCameraModeView::FLyraCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(LYRA_CAMERA_DEFAULT_FOV)
{
}

void FLyraCameraModeView::Blend(const FLyraCameraModeView& Other, float Weight)
{
}

ULyraCameraMode::ULyraCameraMode()
	: FieldOfView(LYRA_CAMERA_DEFAULT_FOV)
	, ViewPitchMin(LYRA_CAMERA_DEFAULT_PITCH_MIN)
	, ViewPitchMax(LYRA_CAMERA_DEFAULT_PITCH_MAX)
	, BlendTime(0.5f)
	, BlendExponent(4.0f)
	, BlendFunction(ELyraCameraModeBlendFunction::EaseOut)
	, BlendAlpha(1.0f)
	, BlendWeight(1.0f)
{
}

ULyraCameraComponent* ULyraCameraMode::GetCameraComponent() const
{
	return CastChecked<ULyraCameraComponent>(GetOuter());
}

AActor* ULyraCameraMode::GetTargetActor() const
{
	return GetCameraComponent()->GetTargetActor();
}

UWorld* ULyraCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

void ULyraCameraMode::Update(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void ULyraCameraMode::SetBlendWeight(float Weight)
{
}

void ULyraCameraMode::UpdateView(float DeltaTime)
{
	FVector PivotLoation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLoation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void ULyraCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		BlendAlpha += DeltaTime / BlendTime;
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case ELyraCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ELyraCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		break;
	}
}

FVector ULyraCameraMode::GetPivotLocation() const
{
	auto TargetActor = GetTargetActor();
	check(TargetActor);

	if (auto TargetPawn = Cast<APawn>(TargetActor))
	{
		if (auto TargetCharacter = Cast<ACharacter>(TargetPawn))
		{
			const auto TargetCharacterClassDefaultObject = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			const float DefaultHalfHeight = TargetCharacterClassDefaultObject->GetCapsuleComponent()
				->GetUnscaledCapsuleHalfHeight();
			const float HalfHeight = TargetCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

			return TargetCharacter->GetActorLocation() + FVector::UpVector + (DefaultHalfHeight - HalfHeight + TargetCharacterClassDefaultObject->BaseEyeHeight);
		}

		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator ULyraCameraMode::GetPivotRotation() const
{
	auto TargetActor = GetTargetActor();
	check(TargetActor);

	if (auto TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}
