// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCharacter.h"
#include "LyraCharacterMovementComponent.h"
#include "LyraPawnComponent.h"
#include "Camera/LyraCameraComponent.h"
#include "Components/CapsuleComponent.h"

ALyraCharacter::ALyraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);

	auto CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(TEXT("LyraPawnCapsule"));

	auto SkeletalMeshComp = GetMesh();
	check(SkeletalMeshComp);
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMeshComp->SetCollisionProfileName(TEXT("LyraPawnMesh"));

	PawnComponent = CreateDefaultSubobject<ULyraPawnComponent>(TEXT("PawnComponent"));
	CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>(TEXT("CameraComponent"));
}

void ALyraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnComponent->OnControllerChanged();
}

void ALyraCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnComponent->OnControllerChanged();
}

void ALyraCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnComponent->OnControllerChanged();
}

void ALyraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnComponent->OnPlayerStateReplicated();
}

void ALyraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnComponent->OnSetupPlayerInputComponent();
}
