// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModularCharacter.h"

#include "LyraCharacter.generated.h"

UCLASS()
class LYRAGAME_API ALyraCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	ALyraCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess="true"))
	TObjectPtr<class ULyraPawnComponent> PawnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ULyraCameraComponent> CameraComponent;
};
