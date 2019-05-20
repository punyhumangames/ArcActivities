// 2017-2019 Puny Human

#include "ActivitiesPluginEditor.h"
#include "ActivityNode_Objective.h"
#include "ActivityObjective.h"
#include "DataModel/ObjectiveTracker.h"
#include "ActivityNode_ObjectiveTracker.h"

#include "AIGraphTypes.h"

#include "SGraphEditorActionMenuAI.h"

#include "EdGraph/EdGraph.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"	

#define LOCTEXT_NAMESPACE "ActivityNode"

UActivityNode_Objective::UActivityNode_Objective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSubNode = true;
	ClassData = FGraphNodeClassData(UActivityObjective::StaticClass(), FString());

	BottomSubNodeClass = UActivityNode_ObjectiveTracker::StaticClass();
}

void UActivityNode_Objective::AllocateDefaultPins()
{
	//No Pins
}

void UActivityNode_Objective::GetContextMenuActions(const FGraphNodeContextMenuBuilder& Context) const
{
	AddContextMenuActionsTracker(Context);
}

UActivityObjective* UActivityNode_Objective::GetObjective()
{
	return Cast<UActivityObjective>(GetNodeInstance());
}

void UActivityNode_Objective::CreateAddTrackerSubMenu(class FMenuBuilder& MenuBuilder, UEdGraph* Graph) const
{
	TSharedRef<SGraphEditorActionMenuAI> Menu =
		SNew(SGraphEditorActionMenuAI)
		.GraphObj(Graph)
		.GraphNode((UActivityNode_Base*)this)
		.SubNodeFlags((uint32)EActivitySubnodeType::ObjectiveTracker)
		.AutoExpandActionMenu(true);

	MenuBuilder.AddWidget(Menu, FText(), true);
}

void UActivityNode_Objective::AddContextMenuActionsTracker(const FGraphNodeContextMenuBuilder& Context) const
{
	Context.MenuBuilder->AddSubMenu(
		LOCTEXT("AddTracker", "Add Tracker..."),
		LOCTEXT("AddTrackerTooltip", "Adds new Tracker as a subnode"),
		FNewMenuDelegate::CreateUObject(this, &UActivityNode_Objective::CreateAddTrackerSubMenu, (UEdGraph*)Context.Graph));
}

void UActivityNode_Objective::OnSubNodeAdded(UAIGraphNode* SubNode)
{
	Super::OnSubNodeAdded(SubNode);

	if (UActivityNode_ObjectiveTracker* ObjectiveTrackerNode = Cast<UActivityNode_ObjectiveTracker>(SubNode))
	{
		GetObjective()->ObjectiveTrackers.AddUnique(ObjectiveTrackerNode->GetObjectiveTracker());
	}
}

void UActivityNode_Objective::OnSubNodeRemoved(UAIGraphNode* SubNode)
{
	Super::OnSubNodeRemoved(SubNode);

	if (UActivityNode_ObjectiveTracker* ObjectiveTrackerNode = Cast<UActivityNode_ObjectiveTracker>(SubNode))
	{
		GetObjective()->ObjectiveTrackers.Remove(ObjectiveTrackerNode->GetObjectiveTracker());
	}
}

#undef LOCTEXT_NAMESPACE