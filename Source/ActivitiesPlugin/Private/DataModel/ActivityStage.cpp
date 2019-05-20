// 2017-2018 Puny Human Games

#include "ActivitiesPlugin.h"
#include "ActivityStage.h"
#include "Activity.h"


UActivityStage::UActivityStage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UActivity* UActivityStage::GetOwningActivity()
{
	return Cast<UActivity>(GetOuter());
}
