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

    UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
    class ALyraPlayerState* GetLyraPlayerState() const;

    UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
    class ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnCameraPenetratingTarget() override;

    virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
protected:
	bool bHideViewTargetPawnNextFrame = false;
};
