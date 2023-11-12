// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info

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
