// Fill out your copyright notice in the Description page of Project Settings.


#include "Cosmetics/LyraCharacterPart.h"
#include "Cosmetics/LyraCharacterPartsPawnComponent.h"

FString FLyraAppliedCharacterPartEntry::GetDebugString() const
{
	return FString::Printf(TEXT("(PartClass:%s, Socket:%s, Instance:%s)"), *GetPathNameSafe(Part.PartClass), *Part.SocketName.ToString(), *GetPathNameSafe(SpawnedComponent));
}

void FLyraCharacterPartList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	bool bDestroyedAnyActors = false;
	for (auto Index : RemovedIndices)
	{
		bDestroyedAnyActors |= DestroyActorForEntry(AppliedEntries[Index]);
	}

	if (bDestroyedAnyActors && ensure(Owner))
	{
		Owner->BroadcastPartsChanged();
	}
}

void FLyraCharacterPartList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	bool bSpawnedAnyActors = false;
	for (auto Index : AddedIndices)
	{
		bSpawnedAnyActors |= SpawnActorForEntry(AppliedEntries[Index]);
	}

	if (bSpawnedAnyActors && ensure(Owner))
	{
		Owner->BroadcastPartsChanged();
	}
}

void FLyraCharacterPartList::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	bool bChangedAnyActors = false;
	for (auto Index : ChangedIndices)
	{
		bChangedAnyActors |= DestroyActorForEntry(AppliedEntries[Index]);
		bChangedAnyActors |= SpawnActorForEntry(AppliedEntries[Index]);
	}

	if (bChangedAnyActors && ensure(Owner))
	{
		Owner->BroadcastPartsChanged();
	}
}

bool FLyraCharacterPartList::SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry)
{
	bool bSpawnAnyActor = false;

	if (ensure(Owner) && !Owner->IsNetMode(NM_DedicatedServer) && Entry.Part.PartClass != nullptr)
	{
		if (auto CompAttachTo = Owner->GetSceneComponentAttachTo())
		{
			const FTransform SpawnTransform = CompAttachTo->GetSocketTransform(Entry.Part.SocketName);

			auto PartComp = NewObject<UChildActorComponent>(Owner->GetOwner());
			PartComp->SetupAttachment(CompAttachTo, Entry.Part.SocketName);
			PartComp->SetChildActorClass(Entry.Part.PartClass);
			PartComp->RegisterComponent();

			if (auto SpawnedActor = PartComp->GetChildActor())
			{
				switch (Entry.Part.CollisionMode)
				{
				case ELyraCharacterCollisionMode::UseCollisionFromCharacterPart:
					break;
				case ELyraCharacterCollisionMode::NoCollision:
					SpawnedActor->SetActorEnableCollision(false);
					break;
				}

				if (auto SpawnedActorRootComp = SpawnedActor->GetRootComponent())
				{
					SpawnedActorRootComp->AddTickPrerequisiteComponent(CompAttachTo);
				}
			}

			Entry.SpawnedComponent = PartComp;
			bSpawnAnyActor = true;
		}
	}
	return bSpawnAnyActor;
}

bool FLyraCharacterPartList::DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent != nullptr)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
		return true;
	}

	return false;
}

FLyraCharacterPartHandle FLyraCharacterPartList::AddEntry(const FLyraCharacterPart& Part)
{
	FLyraCharacterPartHandle Handle;
	Handle.PartHandle = PartHandleNum++;

	if (ensure(Owner && Owner->GetOwner() && Owner->GetOwner()->HasAuthority()))
	{
		auto& Entry = AppliedEntries.AddDefaulted_GetRef();
		Entry.Part = Part;
		Entry.PartHandle = Handle.PartHandle;

		if (SpawnActorForEntry(Entry))
		{
			Owner->BroadcastPartsChanged();
		}

		MarkItemDirty(Entry);
	}

	return Handle;
}

void FLyraCharacterPartList::RemoveEntry(FLyraCharacterPartHandle Part)
{
	for (auto It = AppliedEntries.CreateIterator(); It; ++It)
	{
		if (It->PartHandle == Part.PartHandle)
		{
			const bool bDestroyActor = DestroyActorForEntry(*It);
			It.RemoveCurrent();
			MarkArrayDirty();

			if (bDestroyActor && ensure(Owner))
			{
				Owner->BroadcastPartsChanged();
			}

			break;
		}
	}
}

void FLyraCharacterPartList::RemoveAllEntries(bool bBroadcastChanges)
{
	bool bDestroyAnyActor = false;
	for (auto& Entry : AppliedEntries)
	{
		bDestroyAnyActor |= DestroyActorForEntry(Entry);
	}
	AppliedEntries.Reset();
	MarkArrayDirty();

	if (bDestroyAnyActor && bBroadcastChanges && ensure(Owner))
	{
		Owner->BroadcastPartsChanged();
	}
}
