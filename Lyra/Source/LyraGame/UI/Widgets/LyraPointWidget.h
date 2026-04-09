// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicEntryBoxBase.h"
#include "GameplayTagContainer.h"
#include "LyraPointWidget.generated.h"

struct FPointWidgetRequest;

UENUM(BlueprintType)
enum class EGameplayTagMatchPolicy : uint8
{
    ExactMatch,
    PartialMatch
};

UENUM(BlueprintType)
enum class EPointWidgetAction : uint8
{
    Added,
    Removed
};

DECLARE_DELEGATE_TwoParams(FOnPointWidgetsActionDelegate, EPointWidgetAction Action, const FPointWidgetRequest& Request);

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
	
public:
    ULyraPointWidget(const FObjectInitializer& ObjectInitializer);

    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    FGameplayTag Tag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    EGameplayTagMatchPolicy MatchPolicy = EGameplayTagMatchPolicy::ExactMatch;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
    TArray<TObjectPtr<UClass>> ValidWidgetClasses;
private:
    void RegisterSubWidgets();
    void RegisterSubWidgetsWithPlayerState(class UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);

    void OnWidgetAction(EPointWidgetAction Action, const FPointWidgetRequest& Request);
};
