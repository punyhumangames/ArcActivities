// 2017-2018 Puny Human Games

#include "DataModel/ArcActivityStage.h"
#include "DataModel/ArcActivity.h"


UArcActivityStage::UArcActivityStage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UArcActivity* UArcActivityStage::GetOwningActivity()
{
	return Cast<UArcActivity>(GetOuter());
}
