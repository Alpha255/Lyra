// Fill out your copyright notice in the Description page of Project Settings.


#include "Cosmetics/LyraCharacterPartsControllerComponent.h"
#include "Cosmetics/LyraCharacterPartsPawnComponent.h"

void ULyraCharacterPartsControllerComponent::AddPart(const FLyraCharacterPart& Part)
{
	AddPart(Part, ELyraCharacterPartSource::Natural);
}

void ULyraCharacterPartsControllerComponent::RemovePart(const FLyraCharacterPart& Part)
{
	for (auto It = CharacterParts.CreateIterator(); It; ++It)
	{
		if (FLyraCharacterPart::IsSamePart(Part, It->Part))
		{
			if (auto PawnComp = GetPawnComponent())
			{
				PawnComp->RemovePart(It->Handle);
			}

			It.RemoveCurrent();
			break;
		}
	}
}

void ULyraCharacterPartsControllerComponent::RemoveAllParts()
{
	if (auto PawnComp = GetPawnComponent())
	{
		for (auto& Entry : CharacterParts)
		{
			PawnComp->RemovePart(Entry.Handle);
		}
	}

	CharacterParts.Reset();
}

void ULyraCharacterPartsControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (auto Controller = GetController<AController>())
		{
			Controller->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);

			if (auto Pawn = GetPawn<APawn>())
			{
				OnPossessedPawnChanged(nullptr, Pawn);
			}
		}
	}
}

void ULyraCharacterPartsControllerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllParts();
	Super::EndPlay(EndPlayReason);
}

void ULyraCharacterPartsControllerComponent::AddPart(const FLyraCharacterPart& Part, ELyraCharacterPartSource Source)
{
	auto& Entry = CharacterParts.AddDefaulted_GetRef();
	Entry.Part = Part;
	Entry.Source = Source;

	if (auto PawnComp = GetPawnComponent())
	{
		if (Entry.Source != ELyraCharacterPartSource::NaturalSuppressedViaCheat)
		{
			Entry.Handle = PawnComp->AddPart(Part);
		}
	}
}

ULyraCharacterPartsPawnComponent* ULyraCharacterPartsControllerComponent::GetPawnComponent() const
{
	if (auto Pawn = GetPawn<APawn>())
	{
		return Pawn->FindComponentByClass<ULyraCharacterPartsPawnComponent>();
	}

	return nullptr;
}

void ULyraCharacterPartsControllerComponent::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (auto PawnComp = OldPawn ? OldPawn->FindComponentByClass<ULyraCharacterPartsPawnComponent>() : nullptr)
	{
		for (auto& Entry : CharacterParts)
		{
			PawnComp->RemovePart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	if (auto PawnComp = NewPawn ? NewPawn->FindComponentByClass<ULyraCharacterPartsPawnComponent>() : nullptr)
	{
		for (auto& Entry : CharacterParts)
		{
			if (!Entry.Handle.IsValid() && Entry.Source != ELyraCharacterPartSource::NaturalSuppressedViaCheat)
			{
				Entry.Handle = PawnComp->AddPart(Entry.Part);
			}
		}
	}
}
