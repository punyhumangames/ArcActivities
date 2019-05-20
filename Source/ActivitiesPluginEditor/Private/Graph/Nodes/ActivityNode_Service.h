// 2017-2019 Puny Human

#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Service.generated.h"

class UActivityTask_StageService;

UCLASS()
class UActivityNode_Service : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_Service(const FObjectInitializer& ObjectInitializer);

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void AllocateDefaultPins() override;

	UActivityTask_StageService* GetService();

};