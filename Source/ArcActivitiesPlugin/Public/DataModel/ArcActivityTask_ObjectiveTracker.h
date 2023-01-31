// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataModel/ArcActivityTask_Base.h"
#include "ArcActivityTypes.h"
#include "ArcActivityTask_ObjectiveTracker.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FArcObjectiveTrackerStateUpdated, UArcActivityTask_ObjectiveTracker* /*Tracker*/);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class ARCACTIVITIESPLUGIN_API UArcActivityTask_ObjectiveTracker : public UArcActivityTask_Base
{
	GENERATED_BODY()
public:
	friend class UArcActivityInstance;

	UArcActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);

	UArcActivityObjective* GetOwningObjective() const;
	

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
	TObjectPtr<UArcActivityObjective> ObjectiveRef;

};
