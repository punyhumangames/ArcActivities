// 2017-2019 Puny Human

#pragma once

#include "ActivityNode_Base.h"
#include "ActivityNode_Objective.generated.h"

class UActivityObjective;

UCLASS()
class UActivityNode_Objective : public UActivityNode_Base
{
	GENERATED_BODY()
public:
	UActivityNode_Objective(const FObjectInitializer& ObjectInitializer);

	virtual void AllocateDefaultPins() override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	UActivityObjective* GetObjective() const;

	void CreateAddTrackerSubMenu(class FMenuBuilder& MenuBuilder, UEdGraph* Graph) const;
	void AddContextMenuActionsTracker(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const;

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode);
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode);
};