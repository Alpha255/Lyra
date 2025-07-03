// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraCameraComponent.h"
#include "LyraCameraComponent.h"

ULyraCameraComponent::ULyraCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraCameraComponent::OnRegister()
{
	Super::OnRegister();
}

void ULyraCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
}
