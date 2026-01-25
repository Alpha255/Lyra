#include "LyraEquipmentDefinition.h"
#include "LyraEquipmentInstance.h"

ULyraEquipmentDefinition::ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    EquipmentInstance = ULyraEquipmentInstance::StaticClass();
}