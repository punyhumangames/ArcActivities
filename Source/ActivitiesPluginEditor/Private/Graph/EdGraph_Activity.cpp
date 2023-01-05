// 2017-2019 Puny Human

#include "DataModel/Activity.h"
#include "EdGraphSchema_Activity.h"
#include "GraphEditAction.h"
#include "Nodes/ActivityNode_Entry.h"



const FName FActivityNodePinTypes::Input = TEXT("Input");
const FName FActivityNodePinTypes::Output_Fail = TEXT("Ouput_Fail");
const FName FActivityNodePinTypes::Output_Success = TEXT("Output_Success");

UEdGraph_Activity::UEdGraph_Activity(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Schema = UEdGraphSchema_Activity::StaticClass();
} 
void UEdGraph_Activity::OnCreated()
{
	UActivityNode_Entry* EntryNode = NewObject<UActivityNode_Entry>(GetTransientPackage());
	EntryNode->bIsReadOnly = true;
	EntryNode->Rename(nullptr, this, REN_NonTransactional);

	this->AddNode(EntryNode);

	EntryNode->CreateNewGuid();
	EntryNode->PostPlacedNewNode();
	EntryNode->AllocateDefaultPins();

	EntryNode->NodePosX = 0;
	EntryNode->NodePosY = 0;
	EntryNode->SnapToGrid(GRID_SNAP);
}

void UEdGraph_Activity::RefreshNodeSelection(UEdGraphNode* Node)
{
	TSet<const UEdGraphNode*> NodesToFocus;
	//NodesToFocus.Add(Node);

	FEdGraphEditAction SelectionAction;
	SelectionAction.Action = GRAPHACTION_SelectNode;
	SelectionAction.Graph = this;
	NotifyGraphChanged(SelectionAction);
	SelectionAction.Nodes = NodesToFocus;
	NotifyGraphChanged(SelectionAction);
}

void UEdGraph_Activity::OnSubNodeDropped()
{
	NotifyGraphChanged();
}

UActivity* UEdGraph_Activity::GetActivity()
{
	return CastChecked<UActivity>(GetOuter());
}
