// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "LyraCameraMode.h"

#include "LyraCameraComponent.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_RetVal(TSubclassOf<class ULyraCameraMode>, FLyraCameraModeDelegate);


UCLASS()
class LYRAGAME_API ULyraCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	ULyraCameraComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Lyra|Camera")
	static ULyraCameraComponent* GetCameraComponent(const AActor* Actor)
	{
		return Actor ? Actor->FindComponentByClass<ULyraCameraComponent>() : nullptr;
	}

	virtual AActor* GetTargetActor() const { return GetOwner(); }

	FLyraCameraModeDelegate DetermineCameModeDelegate;
protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();
private:
	UPROPERTY()
	TObjectPtr<class ULyraCameraModeStack> CameraModeStack;

	float FieldOfViewOffset;
};
