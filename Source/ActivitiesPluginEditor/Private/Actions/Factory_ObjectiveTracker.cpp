// 2017-2019 Puny Human

#include "Factory_ObjectiveTracker.h"
#include "DataModel/ObjectiveTracker.h"


UFactory_ObjectiveTracker::UFactory_ObjectiveTracker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UActivityTask_ObjectiveTracker::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UFactory_ObjectiveTracker::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UActivityTask_ObjectiveTracker>(InParent, Class, Name, Flags);
}

bool UFactory_ObjectiveTracker::CanCreateNew() const
{
	return true;
}
