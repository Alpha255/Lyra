// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Cosmetics/LyraCharacterPart.h"
#include "Cosmetics/LyraCosmeticsAnimation.h"

#include "LyraCharacterPartsPawnComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLyraCharacterPartsChanged, class ULyraCharacterPartsPawnComponent*, CharacterPartsPawnComp);

UCLASS(Meta=(BlueprintSpawnableComponent))
class LYRAGAME_API ULyraCharacterPartsPawnComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraCharacterPartsPawnComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRegister() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	FLyraCharacterPartHandle AddPart(const FLyraCharacterPart& Part);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void RemovePart(FLyraCharacterPartHandle Handle);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void RemoveAllParts();

	class USkeletalMeshComponent* GetSkeletalMeshComponent() const;
	USceneComponent* GetSceneComponentAttachTo() const;

	void BroadcastPartsChanged();

	UPROPERTY(BlueprintAssignable, Category="Cosmetics", BlueprintCallable)
	FLyraCharacterPartsChanged OnPartsChanged;
private:
	UPROPERTY(Replicated, Transient)
	FLyraCharacterPartList PartList;

	UPROPERTY(EditAnywhere, Category="Cosmetics")
	FLyraAnimBodyStyleSelectionSet BodyMeshes;
};
