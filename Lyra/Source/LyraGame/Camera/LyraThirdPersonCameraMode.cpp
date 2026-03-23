// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraThirdPersonCameraMode.h"
#include "GameFramework/Character.h"
#include "Curves/CurveVector.h"
#include "Player/LyraPlayerController.h"
#include "GameFramework/CameraBlockingVolume.h"

static const FName NAME_IgnoreCameraCollision = TEXT("IgnoreCameraCollision");

ULyraThirdPersonCameraMode::ULyraThirdPersonCameraMode()
{
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+00.0f, +00.0f, 0.0f), 1.00f, 1.00f, 14.f, 0));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+00.0f, +16.0f, 0.0f), 0.75f, 0.75f, 00.f, 3));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+00.0f, -16.0f, 0.0f), 0.75f, 0.75f, 00.f, 3));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+00.0f, +32.0f, 0.0f), 0.50f, 0.50f, 00.f, 5));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+00.0f, -32.0f, 0.0f), 0.50f, 0.50f, 00.f, 5));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(+20.0f, +00.0f, 0.0f), 1.00f, 1.00f, 00.f, 4));
	PenetrationAvoidanceFeelers.Add(FLyraPenetrationAvoidanceFeeler(FRotator(-20.0f, +00.0f, 0.0f), 0.50f, 0.50f, 00.f, 4));
}

void ULyraThirdPersonCameraMode::UpdateView(float DeltaTime)
{
	UpdateCharacterCrouchOffset(DeltaTime);

	FVector PivotLocation = GetPivotLocation() + CurrentCrouchOffset;
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	if (!bUseRuntimeFloatCurves)
	{
		if (TargetOffsetCurve)
		{
			auto TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
			View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
		}
	}
	else
	{
		FVector TargetOffset(0.0f);
		TargetOffset.X = TargetOffsetX.GetRichCurveConst()->Eval(PivotRotation.Pitch);
		TargetOffset.Y = TargetOffsetY.GetRichCurveConst()->Eval(PivotRotation.Pitch);
		TargetOffset.Z = TargetOffsetZ.GetRichCurveConst()->Eval(PivotRotation.Pitch);

		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}

	UpdatePreventPenetration(DeltaTime);
}

void ULyraThirdPersonCameraMode::UpdateCharacterCrouchOffset(float DeltaTime)
{
	if (auto TargetCharacter = Cast<ACharacter>(GetTargetActor()))
	{
		if (TargetCharacter->bIsCrouched)
		{
			auto TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			SetCharacterCrouchOffset(FVector(0.0f, 0.0f, TargetCharacterCDO->CrouchedEyeHeight - TargetCharacterCDO->BaseEyeHeight));
		}
	}
	else
	{
		SetCharacterCrouchOffset(FVector::ZeroVector);
	}

	if (CrouchOffsetBlendPct < 1.0f)
	{
		CrouchOffsetBlendPct = FMath::Min(CrouchOffsetBlendPct + DeltaTime * CrouchOffsetBlendMultiplier, 1.0f);
		CurrentCrouchOffset = FMath::InterpEaseInOut(InitialCrouchOffset, TargetCrouchOffset, CrouchOffsetBlendPct, 1.0f);
	}
	else
	{
		CurrentCrouchOffset = TargetCrouchOffset;
		CrouchOffsetBlendPct = 1.0f;
	}
}

void ULyraThirdPersonCameraMode::SetCharacterCrouchOffset(FVector TargetOffset)
{
	CrouchOffsetBlendPct = 0.0f;
	InitialCrouchOffset = CurrentCrouchOffset;
	TargetCrouchOffset = TargetOffset;
}

void ULyraThirdPersonCameraMode::UpdatePreventPenetration(float DeltaTime)
{
	if (!bPreventPenetration)
	{
		return;
	}

	auto TargetActor = GetTargetActor();
	auto TargetPawn = Cast<APawn>(TargetActor);
	auto TargetController = TargetPawn ? TargetPawn->GetController() : nullptr;

	auto TargetControllerCAI = Cast<ILyraCameraAssistInterface>(TargetController);
	auto TargetActorCAI = Cast<ILyraCameraAssistInterface>(TargetActor);
	TOptional<AActor*> OptionalTarget = TargetActorCAI ? TargetActorCAI->GetCameraPreventPenetrationTarget() : TOptional<AActor*>();
	AActor* OptionalTargetActor = OptionalTarget.IsSet() ? OptionalTarget.GetValue() : TargetActor;
	ILyraCameraAssistInterface* OptionalCAI = OptionalTarget.IsSet() ? Cast<ILyraCameraAssistInterface>(OptionalTargetActor) : nullptr;

	if (auto OptionalRootComp = Cast<UPrimitiveComponent>(OptionalTargetActor->GetRootComponent()))
	{
		// Attempt at picking SafeLocation automatically, so we reduce camera translation when aiming.
		// Our camera is our reticle, so we want to preserve our aim and keep that as steady and smooth as possible.
		// Pick closest point on capsule to our aim line.
		FVector ClosestPointOnLineToCapsuleCenter;
		FVector ActorLocation = OptionalTargetActor->GetActorLocation();
		FMath::PointDistToLine(ActorLocation, View.Rotation.Vector(), View.Location, ClosestPointOnLineToCapsuleCenter);

		// Adjust Safe distance height to be same as aim line, but within capsule.
		const float PushInDistance = PenetrationAvoidanceFeelers[0].Extent + CollisionPushOutDistance;
		const float MaxHalfHeight = OptionalTargetActor->GetSimpleCollisionHalfHeight() - PushInDistance;
		ActorLocation.Z = FMath::Clamp(ClosestPointOnLineToCapsuleCenter.Z, ActorLocation.Z - MaxHalfHeight, ActorLocation.Z + MaxHalfHeight);

		float DistanceSqrt;
		OptionalRootComp->GetSquaredDistanceToCollision(ClosestPointOnLineToCapsuleCenter, DistanceSqrt, ActorLocation);

		if (!PenetrationAvoidanceFeelers.IsEmpty())
		{
			ActorLocation += (ActorLocation - ClosestPointOnLineToCapsuleCenter).GetSafeNormal() * PushInDistance;
		}

		PreventCameraPenetration(*OptionalTargetActor, ActorLocation, View.Location, DeltaTime, AimLineToDesiredPosBlockedPct, !bDoPredictiveAvoidance);

		ILyraCameraAssistInterface* CAIArray[] = { TargetControllerCAI, TargetActorCAI, OptionalCAI };
		if (AimLineToDesiredPosBlockedPct < ReportPenetrationPercent)
		{
			for (auto CAI : CAIArray)
			{
				if (CAI)
				{
					CAI->OnCameraPenetratingTarget();
				}
			}
		}
	}
}

void ULyraThirdPersonCameraMode::PreventCameraPenetration(const AActor& ViewTarget, const FVector& SafeLocation, FVector& CameraLocation, const float DeltaTime, float& DistBlockedPct, bool bSingleRayOnly)
{
    float HardBlockedPct = DistBlockedPct;
    float SoftBlockedPct = DistBlockedPct;

    float DistBlockedPctLocal = 1.0f;

    FVector BaseRay = CameraLocation - SafeLocation;
    FRotationMatrix BaseRayMatrix(BaseRay.Rotation());
    FVector BaseRayLocalUp, BaseRayLocalForward, BaseRayLocalRight;

    BaseRayMatrix.GetScaledAxes(BaseRayLocalForward, BaseRayLocalRight, BaseRayLocalUp);

    const int32 NumRays = bSingleRayOnly ? FMath::Min(1.0f, PenetrationAvoidanceFeelers.Num()) : PenetrationAvoidanceFeelers.Num();
    FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(CameraPen), false, nullptr);
    CollisionQueryParams.AddIgnoredActor(&ViewTarget);

    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(0.0f);
    auto World = GetWorld();

    for (int32 Index = 0; Index < NumRays; ++Index)
    {
        FLyraPenetrationAvoidanceFeeler& Feeler = PenetrationAvoidanceFeelers[Index];
        if (Feeler.FramesUntilNextTrace <= 0)
        {
            FVector RayTarget;
            {
                FVector RotatedRay = BaseRay.RotateAngleAxis(Feeler.AdjustmentRot.Yaw, BaseRayLocalUp);
                RotatedRay = RotatedRay.RotateAngleAxis(Feeler.AdjustmentRot.Pitch, BaseRayLocalRight);
                RayTarget = SafeLocation + RotatedRay;
            }

            CollisionShape.Sphere.Radius = Feeler.Extent;

            FHitResult HitResult;
            const bool bHit = World->SweepSingleByChannel(HitResult, SafeLocation, RayTarget, FQuat::Identity, ECC_Camera, CollisionShape, CollisionQueryParams);

            Feeler.FramesUntilNextTrace = Feeler.TraceInterval;

            auto const HitActor = HitResult.GetActor();

            if (bHit && HitActor)
            {
                bool bIgnoreHit = false;

                if (HitActor->ActorHasTag(NAME_IgnoreCameraCollision))
                {
                    bIgnoreHit = true;
                    CollisionQueryParams.AddIgnoredActor(HitActor);
                }

                if (!bIgnoreHit && HitActor->IsA<ACameraBlockingVolume>())
                {
                    const FVector ViewTargetForwardXY = ViewTarget.GetActorForwardVector().GetSafeNormal2D();
                    const FVector ViewTargetLocation = ViewTarget.GetActorLocation();
                    const FVector HitOffset = HitResult.Location - ViewTargetLocation;
                    const FVector HitDirectionXY = HitOffset.GetSafeNormal2D();
                    const float DotHitDirection = FVector::DotProduct(ViewTargetForwardXY, HitDirectionXY);
                    if (DotHitDirection > 0.0f)
                    {
                        bIgnoreHit = true;
                        CollisionQueryParams.AddIgnoredActor(HitActor);
                    }
                    else
                    {
                    }
                }

                if (!bIgnoreHit)
                {
                    float const Weight = Cast<APawn>(HitResult.GetActor()) ? Feeler.PawnWeight : Feeler.WorldWeight;
                    float BlockPct = HitResult.Time;
                    BlockPct += (1.0f - BlockPct) * (1.0f - Weight);

                    BlockPct = ((HitResult.Location - SafeLocation).Size() - CollisionPushOutDistance) / (RayTarget - SafeLocation).Size();
                    DistBlockedPctLocal = FMath::Min(BlockPct, DistBlockedPctLocal);
                }
            }

            if (Index == 0)
            {
                HardBlockedPct = DistBlockedPctLocal;
            }
            else
            {
                SoftBlockedPct = DistBlockedPctLocal;
            }
        }
        else
        {
            --Feeler.FramesUntilNextTrace;
        }
    }

    if (bResetInterpolation)
    {
        DistBlockedPct = DistBlockedPctLocal;
    }
    else if (DistBlockedPct < DistBlockedPctLocal)
    {
        if (PenetrationBlendOutTime > DeltaTime)
        {
            DistBlockedPct = DistBlockedPct + DeltaTime / PenetrationBlendOutTime * (DistBlockedPctLocal - DistBlockedPct);
        }
        else
        {
            DistBlockedPct = DistBlockedPctLocal;
        }
    }
    else
    {
        if (DistBlockedPct > HardBlockedPct)
        {
            DistBlockedPct = HardBlockedPct;
        }
        else if (DistBlockedPct > SoftBlockedPct)
        {
            if (PenetrationBlendOutTime > DeltaTime)
            {
                DistBlockedPct = DistBlockedPct - DeltaTime / PenetrationBlendOutTime * (DistBlockedPct - SoftBlockedPct);
            }
            else
            {
                DistBlockedPct = SoftBlockedPct;
            }
        }
    }

    DistBlockedPct = FMath::Clamp<float>(DistBlockedPct, 0.0f, 1.0f);
    if (DistBlockedPct < (1.0f - ZERO_ANIMWEIGHT_THRESH))
    {
        CameraLocation = SafeLocation + (CameraLocation - SafeLocation) * DistBlockedPct;
    }
}
