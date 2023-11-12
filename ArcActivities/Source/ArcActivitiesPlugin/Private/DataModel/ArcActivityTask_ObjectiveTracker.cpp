// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See
// LICENSE-MIT and LICENSE-APACHE for more info

#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "DataModel/ArcActivityObjective.h"

UArcActivityTask_ObjectiveTracker::UArcActivityTask_ObjectiveTracker(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UArcActivityObjective *UArcActivityTask_ObjectiveTracker::GetOwningObjective() const
{
    return Objective;
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
