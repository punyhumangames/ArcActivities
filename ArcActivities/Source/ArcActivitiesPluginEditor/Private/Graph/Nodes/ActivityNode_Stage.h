// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Stage.generated.h"

class UArcActivityStage;
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
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetDescription() const;

	// End UEdGraphNode interface.

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual UArcActivityStage* GetActivityStage() const;

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode);
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode);

protected:	   

	void AddObjectiveNode(UEdGraph* Graph);

};