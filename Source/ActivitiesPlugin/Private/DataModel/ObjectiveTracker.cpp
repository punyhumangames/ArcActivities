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

void UActivityTask_ObjectiveTracker::InitializeTracker_Implementation()
{
}
