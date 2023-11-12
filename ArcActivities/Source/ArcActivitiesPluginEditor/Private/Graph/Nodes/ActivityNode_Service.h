// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Service.generated.h"

class UArcActivityTask_StageService;

UCLASS()
class UActivityNode_Service : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_Service(const FObjectInitializer& ObjectInitializer);

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;

	UArcActivityTask_StageService* GetService();

};