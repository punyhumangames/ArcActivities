// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "ActivityNode_Base.h"
#include "EdGraph/EdGraphNode.h"
#include "ActivityNode_Entry.generated.h"

class UArcActivity;

UCLASS()
class UActivityNode_Entry : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_Entry(const FObjectInitializer& ObjectInitializer);

	// Begin UEdGraphNode interface.
	virtual void AllocateDefaultPins() override;
	virtual const FSlateBrush* GetNodeIcon() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetDescription() const;
	// End UEdGraphNode interface.	 
	
	virtual UObject* GetNodeInstance() const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode);
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode);


};