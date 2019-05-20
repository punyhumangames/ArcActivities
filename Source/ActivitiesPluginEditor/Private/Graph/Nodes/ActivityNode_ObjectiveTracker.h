// 2017-2019 Puny Human

#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_ObjectiveTracker.generated.h"

class UActivityTask_ObjectiveTracker;;

UCLASS()
class UActivityNode_ObjectiveTracker : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;

	UActivityTask_ObjectiveTracker* GetObjectiveTracker();

};