// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/LyraCameraMode.h"
#include "LyraThirdPersonCameraMode.generated.h"

/**
 * 
 */


USTRUCT()
struct FLyraPenetrationAvoidanceFeeler
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "PenetrationAvoidanceFeeler")
	FRotator AdjustmentRot;

	UPROPERTY(EditAnywhere, Category = "PenetrationAvoidanceFeeler")
	float WorldWeight;

	UPROPERTY(EditAnywhere, Category = "PenetrationAvoidanceFeeler")
	float PawnWeight;

	UPROPERTY(EditAnywhere, Category = "PenetrationAvoidanceFeeler")
	float Extent;

	UPROPERTY(EditAnywhere, Category = "PenetrationAvoidanceFeeler")
	int32 TraceInterval;

	UPROPERTY(Transient)
	int32 FramesUntilNextTrace;

	FLyraPenetrationAvoidanceFeeler()
		: AdjustmentRot(ForceInit)
		, WorldWeight(0.0f)
		, PawnWeight(0.0f)
		, Extent(0.0f)
		, TraceInterval(0)
		, FramesUntilNextTrace(0)
	{
	}

	FLyraPenetrationAvoidanceFeeler(const FRotator& InAdjustmentRot,
		const float InWorldWeight,
		const float InPawnWeight,
		const float InExtent,
		const int32 InTraceInterval = 0,
		const int32 InFramesUntilNextTrace = 0)
		: AdjustmentRot(InAdjustmentRot)
		, WorldWeight(InWorldWeight)
		, PawnWeight(InPawnWeight)
		, Extent(InExtent)
		, TraceInterval(InTraceInterval)
		, FramesUntilNextTrace(InFramesUntilNextTrace)
	{
	}
};


UCLASS(Abstract, Blueprintable)
class LYRAGAME_API ULyraThirdPersonCameraMode : public ULyraCameraMode
{
	GENERATED_BODY()
	
public:
	ULyraThirdPersonCameraMode();
protected:
	virtual void UpdateView(float DeltaTime) override;

	void UpdateCharacterCrouchOffset(float DeltaTime);
	void SetCharacterCrouchOffset(FVector TargetOffset);

	void UpdatePreventPenetration(float DeltaTime);
	void PreventCameraPenetration(const AActor& ViewTarget, const FVector& SafeLocation, FVector& CameraLocation, const float DeltaTime, float& DistBlockedPct, bool bSingleRayOnly);

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	bool bUseRuntimeFloatCurves = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Third Person")
	float CrouchOffsetBlendMultiplier = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditConditon = "!bUseRuntimeFloatCurves"))
	TObjectPtr<const class UCurveVector> TargetOffsetCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person", Meta = (EditCondition = "bUseRuntimeFloatCurves"))
	FRuntimeFloatCurve TargetOffsetZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float PenetrationBlendTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float PenetrationBlendOutTime = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bPreventPenetration = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bDoPredictiveAvoidance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float CollisionPushOutDistance = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	float ReportPenetrationPercent = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TArray<FLyraPenetrationAvoidanceFeeler> PenetrationAvoidanceFeelers;

	UPROPERTY(Transient)
	float AimLineToDesiredPosBlockedPct;

	UPROPERTY(Transient)
	TArray<TObjectPtr<const AActor>> DebugActorsHitDuringCameraPenetration;

	float CrouchOffsetBlendPct = 1.0f;
	FVector InitialCrouchOffset = FVector::ZeroVector;
	FVector TargetCrouchOffset = FVector::ZeroVector;
	FVector CurrentCrouchOffset = FVector::ZeroVector;
};
