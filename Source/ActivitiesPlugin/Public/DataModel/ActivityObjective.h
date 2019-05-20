// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "ActivityObjective.generated.h"

class UActivityTask_ObjectiveTracker;

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UActivityObjective  : public UObject
{
	GENERATED_BODY()
public:
	UActivityObjective(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category="Setup")
	FText ObjectiveName;

	UPROPERTY()
	TArray<UActivityTask_ObjectiveTracker*> ObjectiveTrackers;
};
