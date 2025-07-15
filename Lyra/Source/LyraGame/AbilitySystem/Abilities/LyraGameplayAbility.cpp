// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

AController* ULyraGameplayAbility::GetController() const
{
    if (CurrentActorInfo)
    {
        if (auto Controller = CurrentActorInfo->PlayerController.Get())
        {
            return Controller;
        }

        AActor* Owner = CurrentActorInfo->OwnerActor.Get();
        while (Owner)
        {
            if (auto Controller = Cast<AController>(Owner))
            {
                return Controller;
            }

            if (auto Pawn = Cast<APawn>(Owner))
            {
                return Pawn->GetController();
            }

            Owner = Owner->GetOwner();
        }
    }

    return nullptr;
}