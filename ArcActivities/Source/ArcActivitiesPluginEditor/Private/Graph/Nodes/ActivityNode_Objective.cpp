// Copyright 2023 Puny Human.  All Rights Reserved

#include "ActivityNode_Objective.h"
#include "DataModel/ArcActivityObjective.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "ActivityNode_ObjectiveTracker.h"

#include "AIGraphTypes.h"
#include "ToolMenus.h"

#include "SGraphEditorActionMenuAI.h"

#include "EdGraph/EdGraph.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"	

#define LOCTEXT_NAMESPACE "ActivityNode"

UActivityNode_Objective::UActivityNode_Objective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsSubNode = true;
	ArcClassData = FArcGraphNodeClassData(UArcActivityObjective::StaticClass(), FString());

	BottomSubNodeClass = UActivityNode_ObjectiveTracker::StaticClass();
}

void UActivityNode_Objective::AllocateDefaultPins()
{
	//No Pins
}

void UActivityNode_Objective::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	AddContextMenuActionsTracker(Menu, Context);
}

FText UActivityNode_Objective::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	switch (TitleType)
	{
	case ENodeTitleType::FullTitle:
	{
		FText Title = IsValid(GetObjective()) ? GetObjective()->ObjectiveName : LOCTEXT("NullObjectiveName", "ERROR: NULL OBJECTIVE");
	
		if (Title.IsEmptyOrWhitespace())
		{
			return LOCTEXT("EmptyObjectiveName", "New Objective");
		}
		return Title;
	}
	default:
		return Super::GetNodeTitle(TitleType);
	}
}

FText UActivityNode_Objective::GetDescription() const
{
	return LOCTEXT("ObjectiveNodeDescription", "An Objective");
}

UArcActivityObjective* UActivityNode_Objective::GetObjective() const
{
	return Cast<UArcActivityObjective>(GetNodeInstance());
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

void UActivityNode_Objective::AddContextMenuActionsTracker(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection("Trackers");
	Section.AddSubMenu("AddTracker",
		LOCTEXT("AddTracker", "Add Tracker..."),
		LOCTEXT("AddTrackerTooltip", "Adds new Tracker as a subnode"),
		FNewMenuDelegate::CreateUObject(this, &UActivityNode_Objective::CreateAddTrackerSubMenu, (UEdGraph*)Context->Graph));
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