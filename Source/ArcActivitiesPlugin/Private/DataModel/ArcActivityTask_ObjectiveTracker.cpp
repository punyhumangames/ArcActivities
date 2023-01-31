// 2017-2018 Puny Human Games

#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "DataModel/ArcActivityObjective.h"

UArcActivityTask_ObjectiveTracker::UArcActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UArcActivityObjective* UArcActivityTask_ObjectiveTracker::GetOwningObjective() const
{
	return ObjectiveRef;
}

void UArcActivityTask_ObjectiveTracker::MarkSuccess()
{
	State = EArcActivityObjectiveTrackerState::CompletedSuccess;
	NotifyActivityInstance();
}

void UArcActivityTask_ObjectiveTracker::MarkFailure()
{
	State = EArcActivityObjectiveTrackerState::CompletedFail;
	NotifyActivityInstance();
}

void UArcActivityTask_ObjectiveTracker::NotifyActivityInstance()
{
	OnTrackerStateUpdated.Broadcast(this);
}
