// Copyright 2023 Puny Human.  All Rights Reserved

#include "EdGraphSchema_Activity.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraph.h"
#include "GraphEditorActions.h"

#include "ActivitiesPluginEditorModule.h"

#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/GenericCommands.h"
#include "ScopedTransaction.h"

#include "EdGraph_Activity.h"
#include "Nodes/ActivityNode_Stage.h"
#include "Nodes/ActivityNode_Entry.h"
#include "Nodes/ActivityNode_Objective.h"
#include "Nodes/ActivityNode_Service.h"
#include "Nodes/ActivityNode_ObjectiveTracker.h"

#include "DataModel/ArcActivityTask_StageService.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"

#define LOCTEXT_NAMESPACE "ActivityEditor"

// Maximum distance a drag can be off a node edge to require 'push off' from node
const int32 NodeDistance = 60;

UEdGraphSchema_Activity::UEdGraphSchema_Activity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UEdGraphSchema_Activity::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	auto Action = AddNewNodeAction(ContextMenuBuilder, LOCTEXT("AddStageCategory", "Add Stage"), LOCTEXT("AddStageMenuDesc", "Add Stage"), FText());
	Action->NodeTemplate = NewObject<UActivityNode_Stage>(ContextMenuBuilder.OwnerOfTemporaries);
																							 	
	Super::GetGraphContextActions(ContextMenuBuilder);
}


const FPinConnectionResponse UEdGraphSchema_Activity::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't Connect to the same Node"));
	}
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Not allowed"));
	}

	// check for cycles
	FNodeVisitorCycleChecker CycleChecker;
	if (!CycleChecker.CheckForLoop(A->GetOwningNode(), B->GetOwningNode()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorcycle", "Can't create a graph cycle"));
	}

	if (A->Direction == EGPD_Output && A->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Replace Connection"));
	}
	if (B->Direction == EGPD_Output && B->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT("Replace Connection"));
	}


	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UEdGraphSchema_Activity::GetArcSubNodeClasses(int32 SubNodeFlags, TArray<FArcGraphNodeClassData>& ClassData, UClass*& GraphNodeClass) const
{
	FActivitiesPluginEditorModule& Module = FModuleManager::GetModuleChecked<FActivitiesPluginEditorModule>(TEXT("ArcActivitiesPluginEditor"));
	FArcGraphNodeClassHelper* ClassCache = Module.GetClassCache().Get();

	if ((EActivitySubnodeType)SubNodeFlags == EActivitySubnodeType::Service)
	{
		ClassCache->GatherClasses(UArcActivityTask_StageService::StaticClass(), ClassData);
		GraphNodeClass = UActivityNode_Service::StaticClass();
	}
	if ((EActivitySubnodeType)SubNodeFlags == EActivitySubnodeType::ObjectiveTracker)
	{
		ClassCache->GatherClasses(UArcActivityTask_ObjectiveTracker::StaticClass(), ClassData);
		GraphNodeClass = UActivityNode_ObjectiveTracker::StaticClass();
	}
}

void UEdGraphSchema_Activity::GetGraphNodeContextActions(FGraphContextMenuBuilder& ContextMenuBuilder, int32 SubNodeFlags) const
{
	UEdGraph* Graph = (UEdGraph*)ContextMenuBuilder.CurrentGraph;
	UClass* GraphNodeClass = nullptr;
	TArray<FArcGraphNodeClassData> NodeClasses;
	GetArcSubNodeClasses(SubNodeFlags, NodeClasses, GraphNodeClass);

	if (GraphNodeClass)
	{
		for (const auto& NodeClass : NodeClasses)
		{
			const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClass.ToString(), false));

			UActivityNode_Base* OpNode = NewObject<UActivityNode_Base>(Graph, GraphNodeClass);
			OpNode->ArcClassData = NodeClass;

			TSharedPtr<FAISchemaAction_NewSubNode> AddOpAction = UAIGraphSchema::AddNewSubNodeAction(ContextMenuBuilder, NodeClass.GetCategory(), NodeTypeName, FText::GetEmpty());
			AddOpAction->ParentNode = Cast<UActivityNode_Base>(ContextMenuBuilder.SelectedObjects[0]);
			AddOpAction->NodeTemplate = OpNode;
		}
	}
}

FLinearColor UEdGraphSchema_Activity::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	if (PinType.PinCategory == FActivityNodePinTypes::Output_Success)
	{
		return FColor::Green;
	}

	if (PinType.PinCategory == FActivityNodePinTypes::Output_Fail)
	{
		return FColor::Orange;
	}


	return FColor::Purple;
}

bool UEdGraphSchema_Activity::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	if (!Super::TryCreateConnection(A, B))
	{
		return false;
	}

	UEdGraphPin* OutputPin = (A->Direction == EEdGraphPinDirection::EGPD_Output) ? A : B;
	UEdGraphNode* Node = OutputPin->GetOwningNode();
	if (Node)
	{
		Node->GetGraph()->NotifyGraphChanged();
	}


	return true;
}	
#undef LOCTEXT_NAMESPACE