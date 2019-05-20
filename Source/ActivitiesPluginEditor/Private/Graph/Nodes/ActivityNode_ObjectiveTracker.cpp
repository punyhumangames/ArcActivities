// 2017-2019 Puny Human

#include "ActivitiesPluginEditor.h"
#include "ActivityNode_ObjectiveTracker.h"
#include "DataModel/ObjectiveTracker.h"

UActivityNode_ObjectiveTracker::UActivityNode_ObjectiveTracker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSubNode = true;
}

FText UActivityNode_ObjectiveTracker::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString StoredClassName = ClassData.GetClassName();
	StoredClassName.RemoveFromEnd(TEXT("_C"));

	return FText::Format(NSLOCTEXT("ActivityClass", "ObjectiveTrackerString", "Tracker {0}"), FText::FromString(StoredClassName));
}

void UActivityNode_ObjectiveTracker::AllocateDefaultPins()
{
	//No Pins
}

UActivityTask_ObjectiveTracker* UActivityNode_ObjectiveTracker::GetObjectiveTracker()
{
	return Cast<UActivityTask_ObjectiveTracker>(GetNodeInstance());
}
