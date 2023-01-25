// 2017-2018 Puny Human Games

#include "DataModel/ObjectiveTracker.h"
#include "DataModel/ActivityObjective.h"

UActivityTask_ObjectiveTracker::UActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UActivityObjective* UActivityTask_ObjectiveTracker::GetOwningObjective() const
{
	return ObjectiveRef;
}

void UActivityTask_ObjectiveTracker::MarkSuccess()
{
	State = EArcActivityObjectiveTrackerState::CompletedSuccess;
	NotifyActivityInstance();
}

void UActivityTask_ObjectiveTracker::MarkFailure()
{
	State = EArcActivityObjectiveTrackerState::CompletedFail;
	NotifyActivityInstance();
}

void UActivityTask_ObjectiveTracker::NotifyActivityInstance()
{
	OnTrackerStateUpdated.Broadcast(this);
}
