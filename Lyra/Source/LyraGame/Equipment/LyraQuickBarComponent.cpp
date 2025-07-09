// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraQuickBarComponent.h"

ULyraQuickBarComponent::ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}