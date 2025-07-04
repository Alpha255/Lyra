// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraCameraModeStack.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	bool IsActive() const { return bIsActive; }

	void Activate();
	void Deactivate();

	void PushCameraMode(TSubclassOf<class ULyraCameraMode> CameraModeClass);

	bool Evaluate(float DeltaTime, struct FLyraCameraModeView& OutCameraModeView);
private:
	class ULyraCameraMode* GetCameraModeInstance(TSubclassOf<class ULyraCameraMode> CameraModeClass);

	void Update(float DeltaTime);
	void Blend(struct FLyraCameraModeView& OutCameraModeView);

	bool bIsActive = true;

	UPROPERTY()
	TArray<TObjectPtr<class ULyraCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<class ULyraCameraMode>> CameraModes;
};
