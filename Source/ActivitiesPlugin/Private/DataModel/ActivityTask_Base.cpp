// 2017-2018 Puny Human Games

#include "DataModel/ActivityTask_Base.h"
#include "DataModel/Activity.h"
#include "ArcActivityInstance.h"



UActivityTask_Base::UActivityTask_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
UWorld *UActivityTask_Base::GetWorld() const
{
    if(UArcActivityInstance* Instance = GetActivityInstance())
	{
		return Instance->GetWorld();
	}
	return nullptr;
}

UArcActivityInstance* UActivityTask_Base::GetActivityInstance() const
{
	return GetTypedOuter<UArcActivityInstance>();
}

UActivity* UActivityTask_Base::GetActivityAsset() const
{
	return ActivityAsset;
}
