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

    auto LyraMoveComp = CastChecked<ULyraCharacterMovementComponent>(GetCharacterMovement());
    LyraMoveComp->GravityScale = 1.0f;
    LyraMoveComp->MaxAcceleration = 2400.0f;
    LyraMoveComp->BrakingFrictionFactor = 1.0f;
    LyraMoveComp->BrakingFriction = 6.0f;
    LyraMoveComp->GroundFriction = 8.0f;
    LyraMoveComp->BrakingDecelerationWalking = 1400.0f;
    LyraMoveComp->bUseControllerDesiredRotation = false;
    LyraMoveComp->bOrientRotationToMovement = false;
    LyraMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
    LyraMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
    LyraMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
    LyraMoveComp->bCanWalkOffLedgesWhenCrouching = true;
    LyraMoveComp->SetCrouchedHalfHeight(65.0f);

	PawnComponent = CreateDefaultSubobject<ULyraPawnComponent>(TEXT("PawnComponent"));
	CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

    BaseEyeHeight = 80.0f;
    CrouchedEyeHeight = 50.0f;
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
