// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Cosmetics/LyraCharacterPart.h"

#include "LyraCharacterPartsControllerComponent.generated.h"

/**
 * 
 */


UCLASS(Meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraCharacterPartsControllerComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void AddPart(const FLyraCharacterPart& Part);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void RemovePart(const FLyraCharacterPart& Part);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void RemoveAllParts();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	UPROPERTY(EditAnywhere, Category="Cosmetics")
	TArray<FLyraCharacterPartEntry> CharacterParts;

private:
	void AddPart(const FLyraCharacterPart& Part, ELyraCharacterPartSource Source);

	class ULyraCharacterPartsPawnComponent* GetPawnComponent() const;

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
