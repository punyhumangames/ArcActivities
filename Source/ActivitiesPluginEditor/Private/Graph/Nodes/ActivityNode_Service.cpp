// 2017-2019 Puny Human

#include "ActivitiesPluginEditor.h"
#include "ActivityNode_Service.h"
#include "DataModel/ActivityTask_StageService.h"

UActivityNode_Service::UActivityNode_Service(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FText UActivityNode_Service::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString StoredClassName = ClassData.GetClassName();
	StoredClassName.RemoveFromEnd(TEXT("_C"));

	return FText::Format(NSLOCTEXT("ActivityClass", "ObjectiveTrackerString", "Service {0}"), FText::FromString(StoredClassName));

}

void UActivityNode_Service::AllocateDefaultPins()
{
	//No Pins
}

UActivityTask_StageService* UActivityNode_Service::GetService()
{
	return Cast<UActivityTask_StageService>(GetNodeInstance());
}
