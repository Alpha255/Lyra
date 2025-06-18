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
	return false;
}

bool FLyraCharacterPartList::DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry)
{
	return false;
}

FLyraCharacterPartHandle FLyraCharacterPartList::AddEntry(const FLyraCharacterPart& Part)
{
	return FLyraCharacterPartHandle();
}

void FLyraCharacterPartList::RemoveEntry(const FLyraCharacterPart& Part)
{
}

void FLyraCharacterPartList::RemoveAllEntries(bool bBroadcastChanges)
{
}
