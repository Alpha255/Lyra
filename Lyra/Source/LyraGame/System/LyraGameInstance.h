// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "LyraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:
	ULyraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	virtual void Init() override;
};
