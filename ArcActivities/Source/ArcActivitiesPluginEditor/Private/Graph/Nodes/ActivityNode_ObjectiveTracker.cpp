// Copyright 2023 Puny Human.  All Rights Reserved

#include "ActivityNode_ObjectiveTracker.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"

UActivityNode_ObjectiveTracker::UActivityNode_ObjectiveTracker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSubNode = true;
}

FText UActivityNode_ObjectiveTracker::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("ActivityClass", "ObjectiveTrackerTitle", "Tracker");
}

FText UActivityNode_ObjectiveTracker::GetDescription() const
{
	FString StoredClassName = ArcClassData.GetClassName();
	StoredClassName.RemoveFromEnd(TEXT("_C"));

	return FText::Format(NSLOCTEXT("ActivityClass", "ObjectiveTrackerString", "{0}"), FText::FromString(StoredClassName));
}

void UActivityNode_ObjectiveTracker::AllocateDefaultPins()
{
	//No Pins
}

UArcActivityTask_ObjectiveTracker* UActivityNode_ObjectiveTracker::GetObjectiveTracker()
{
	return Cast<UArcActivityTask_ObjectiveTracker>(GetNodeInstance());
}
