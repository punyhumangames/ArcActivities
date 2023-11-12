// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info

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
