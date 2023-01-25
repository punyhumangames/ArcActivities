// 2017-2019 Puny Human

#pragma once

#include "CoreMinimal.h"
#include "ActivityTask_Base.h"
#include "ArcActivityTypes.h"
#include "ObjectiveTracker.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FArcObjectiveTrackerStateUpdated, UActivityTask_ObjectiveTracker* /*Tracker*/);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class ACTIVITIESPLUGIN_API UActivityTask_ObjectiveTracker : public UActivityTask_Base
{
	GENERATED_BODY()
public:
	friend class UArcActivityInstance;

	UActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);

	UActivityObjective* GetOwningObjective() const;
	

	UFUNCTION(BlueprintPure, Category="Acitivty")
	EArcActivityObjectiveTrackerState GetTrackerState() const { return State; }

	UFUNCTION(BlueprintCallable, Category = "Acitivty")
	void MarkSuccess();

	UFUNCTION(BlueprintCallable, Category = "Acitivty")
	void MarkFailure();

	FArcObjectiveTrackerStateUpdated OnTrackerStateUpdated;

private:

	void NotifyActivityInstance();

	EArcActivityObjectiveTrackerState State;


	UPROPERTY()
	TObjectPtr<UActivityObjective> ObjectiveRef;

};
