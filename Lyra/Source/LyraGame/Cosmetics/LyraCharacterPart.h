// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "LyraCharacterPart.generated.h"

enum class ELyraCharacterPartSource : uint8
{
	Natural,
	NaturalSuppressedViaCheat,
	AppliedViaDeveloperSettingsCheat,
	AppliedViaCheatManager
};

UENUM()
enum class ELyraCharacterCollisionMode : uint8
{
	NoCollision,
	UseCollisionFromCharacterPart
};

USTRUCT(BlueprintType)
struct FLyraCharacterPartHandle
{
	GENERATED_BODY()

	void Reset() { PartHandle = INDEX_NONE; }

	bool IsValid() const { return PartHandle != INDEX_NONE; }
private:
	int32 PartHandle = INDEX_NONE;

	friend struct FLyraCharacterPartList;
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

USTRUCT()
struct FLyraCharacterPartEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Meta = (ShowOnlyInnerProperties))
	FLyraCharacterPart Part;

	FLyraCharacterPartHandle Handle;

	ELyraCharacterPartSource Source = ELyraCharacterPartSource::Natural;
};

USTRUCT()
struct FLyraAppliedCharacterPartEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FString GetDebugString() const;
private:
	friend struct FLyraCharacterPartList;
	friend class ULyraCharacterPartsPawnComponent;

private:
	UPROPERTY()
	FLyraCharacterPart Part;

	// Server only
	UPROPERTY(NotReplicated)
	int32 PartHandle = INDEX_NONE;
	
	// Client only
	UPROPERTY(NotReplicated)
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FLyraCharacterPartList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLyraAppliedCharacterPartEntry, FLyraCharacterPartList>(AppliedEntries, DeltaParams, *this);
	}

	FLyraCharacterPartHandle AddEntry(const FLyraCharacterPart& Part);
	void RemoveEntry(const FLyraCharacterPart& Part);
	void RemoveAllEntries(bool bBroadcastChanges);
private:
	friend class ULyraCharacterPartsPawnComponent;

	bool SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry);
	bool DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry);

	UPROPERTY()
	TArray<FLyraAppliedCharacterPartEntry> AppliedEntries;

	UPROPERTY(NotReplicated)
	TObjectPtr<class ULyraCharacterPartsPawnComponent> Owner = nullptr;

	int32 PartHandleNum = 0;
};

template<>
struct TStructOpsTypeTraits<FLyraCharacterPartList> : public TStructOpsTypeTraitsBase2<FLyraCharacterPartList>
{
	enum { WithNetDeltaSerializer = true };
};
