// 2017-2019 Puny Human

#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Stage.generated.h"

class UActivityStage;
class UActivityNode_Objective;

UCLASS()
class UActivityNode_Stage : public UActivityNode_Base
{
	GENERATED_BODY()
public:	   	  	
	UActivityNode_Stage(const FObjectInitializer& ObjectInitializer);

	
	// Begin UEdGraphNode interface.
	virtual void AllocateDefaultPins() override;
	virtual const FSlateBrush* GetNodeIcon() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	// End UEdGraphNode interface.

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual UActivityStage* GetActivityStage();

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode);
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode);

protected:	   

	void AddObjectiveNode(UEdGraph* Graph);

};