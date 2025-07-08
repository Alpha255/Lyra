// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraCameraMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELyraCameraModeBlendFunction :uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	Num UMETA(Hidden)
};

struct FLyraCameraModeView
{
	FLyraCameraModeView();

	void Blend(const FLyraCameraModeView& Other, float Weight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UCLASS(Abstract, NotBlueprintable)
class LYRAGAME_API ULyraCameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	ULyraCameraMode();

	class ULyraCameraComponent* GetCameraComponent() const;
	AActor* GetTargetActor() const;

	virtual UWorld* GetWorld() const override;

	virtual void Activate() {}
	virtual void Deactivate() {}

	const FLyraCameraModeView& GetCameraModeView() const { return View; }

	float GetBlendWeight() const { return BlendWeight; }
	float GetBlendTime() const { return BlendTime; }
	void SetBlendWeight(float Weight);

	void Update(float DeltaTime);

protected:
	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

	UPROPERTY(EditDefaultsOnly, Category="View", Meta=(UIMin="5.0", UIMax="170.0", ClampMin="5.0", ClampMax="170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category="View", Meta=(UIMin="-90.0", UIMax="90.0", ClampMin="-90.0", ClampMax="90.0"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category="View", Meta=(UIMin="-90.0", UIMax="90.0", ClampMin="-90.0", ClampMax="90.0"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	ELyraCameraModeBlendFunction BlendFunction;

	FLyraCameraModeView View;

	float BlendAlpha;

	float BlendWeight;
};
