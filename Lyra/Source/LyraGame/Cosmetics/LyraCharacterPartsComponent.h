// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraCharacterPartsComponent.generated.h"

/**
 * 
 */

UENUM()
enum class ELyraCharacterCollisionMode : uint8
{
	NoCollision,
	UseCollisionFromCharacterPart
};

USTRUCT(BlueprintType)
struct FLyraCharacterPart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELyraCharacterCollisionMode CollisionMode;

	static bool IsSamePart(const FLyraCharacterPart& A, const FLyraCharacterPart& B)
	{
		return A.PartClass == B.PartClass && A.SocketName == B.SocketName;
	}
};


UCLASS()
class LYRAGAME_API ULyraCharacterPartsComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Cosmetics")
	void AddPart(const FLyraCharacterPart& Part);
};
