#pragma once

#include "Templates/SubclassOf.h"
#include "LyraEquipmentDefinition.generated.h"

USTRUCT()
struct FLyraEquipmentToSpawn
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Equipment")
    TSubclassOf<class AActor> ActorToSpawn;

    UPROPERTY(EditAnywhere, Category = "Equipment")
    FName AttachSocket;

    UPROPERTY(EditAnywhere, Category = "Equipment")
    FTransform AttachTransform;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class ULyraEquipmentDefinition : public UObject
{
    GENERATED_BODY()

public:
    ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditDefaultsOnly, Category = "Equipment")
    TSubclassOf<class ULyraEquipmentInstance> EquipmentInstance;

    UPROPERTY(EditDefaultsOnly, Category = "Equipment")
    TArray<TObjectPtr<const class ULyraGameplayAbilitySet>> AbilitySets;

    UPROPERTY(EditDefaultsOnly, Category = "Equipment")
    TArray<FLyraEquipmentToSpawn> EquipmentsToSpawn;
};