// 2017-2019 Puny Human

#pragma once

#include "CoreMinimal.h"
#include "ActivityTask_Base.h"
#include "ObjectiveTracker.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class ACTIVITIESPLUGIN_API UActivityTask_ObjectiveTracker : public UActivityTask_Base
{
	GENERATED_BODY()
public:
	UActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintNativeEvent)
	void InitializeTracker(class UActivityObjective* Objective);
	virtual void InitializeTracker_Implementation(class UActivityObjective* Objective);


};
