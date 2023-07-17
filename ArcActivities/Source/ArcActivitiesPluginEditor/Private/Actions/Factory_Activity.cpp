// Copyright 2023 Puny Human.  All Rights Reserved

#include "Factory_Activity.h"


UFactory_Activity::UFactory_Activity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UArcActivity::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UFactory_Activity::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UArcActivity>(InParent, Class, Name, Flags);
}

bool UFactory_Activity::CanCreateNew() const
{
	return true;
}
