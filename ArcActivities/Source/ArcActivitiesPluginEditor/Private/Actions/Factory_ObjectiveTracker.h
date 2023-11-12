// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "Engine/DataAsset.h"
#include "Factories/Factory.h"
#include "Factory_ObjectiveTracker.generated.h"


UCLASS()
class UFactory_ObjectiveTracker : public UFactory
{
	GENERATED_BODY()
public:
	UFactory_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	// End of UFactory interface

};