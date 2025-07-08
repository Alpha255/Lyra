// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "Camera/LyraCameraAssistInterface.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ALyraPlayerController : public ACommonPlayerController, public ILyraCameraAssistInterface
{
	GENERATED_BODY()
	
public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnCameraPenetratingTarget() override;

protected:
	bool bHideViewTargetPawnNextFrame = false;
};
