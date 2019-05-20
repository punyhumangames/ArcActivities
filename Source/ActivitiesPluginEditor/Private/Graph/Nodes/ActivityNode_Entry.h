// 2017-2019 Puny Human

#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Entry.generated.h"

class UActivity;

UCLASS()
class UActivityNode_Entry : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_Entry(const FObjectInitializer& ObjectInitializer);

	// Begin UEdGraphNode interface.
	virtual void AllocateDefaultPins() override;
	virtual const FSlateBrush* GetNodeIcon() const override;
	virtual void GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const override;
	// End UEdGraphNode interface.	 
	
	virtual UObject* GetNodeInstance() override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode);
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode);


};