#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (!CurrentPolicy && !DefaultPolicyClass.IsNull())
    {
        TSubclassOf<UGameUIPolicy> PolicyClass = DefaultPolicyClass.LoadSynchronous();
        SetUIPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
    }
}

void UGameUIManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();

    SetUIPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject * Outer) const
{
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
    {
        TArray<UClass*> ChildrenClasses;
        GetDerivedClasses(GetClass(), ChildrenClasses, false);

        return ChildrenClasses.Num() == 0;
    }

    return false;
}

void UGameUIManagerSubsystem::SetUIPolicy(UGameUIPolicy* Policy)
{
    if (CurrentPolicy != Policy)
    {
        CurrentPolicy = Policy;
    }
}
