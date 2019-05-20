// 2017-2019 Puny Human

#pragma once

#include "Engine/DataAsset.h"
#include "Factories/Factory.h"
#include "Factory_Activity.generated.h"


UCLASS()
class UFactory_Activity : public UFactory
{
	GENERATED_BODY()
public:
	UFactory_Activity(const FObjectInitializer& ObjectInitializer);

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	// End of UFactory interface

};