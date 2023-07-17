// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "ArcActivityTypes.h"
#include "ArcActivityObjective.generated.h"

class UArcActivityTask_ObjectiveTracker;



/**
 * 
 */
UCLASS(BlueprintType)
class ARCACTIVITIESPLUGIN_API UArcActivityObjective  : public UObject
{
	GENERATED_BODY()
public:
	UArcActivityObjective(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	FText ObjectiveName;

	UPROPERTY()
	TArray<UArcActivityTask_ObjectiveTracker*> ObjectiveTrackers;

	UPROPERTY(EditAnywhere, Category = "Setup")
	EArcActivityCompletionMode CompletionMode;
};
