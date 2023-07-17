// Copyright 2023 Puny Human.  All Rights Reserved

#include "ActivityNode_Entry.h"
#include "Graph/EdGraph_Activity.h"
#include "EdGraph/EdGraph.h"

#include "ActivityNode_Service.h"

#include "DataModel/ArcActivity.h"
#include "DataModel/ArcActivityStage.h"

#define LOCTEXT_NAMESPACE "ActivityNode"

UActivityNode_Entry::UActivityNode_Entry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TopSubNodeClass = UActivityNode_Service::StaticClass();
}

void UActivityNode_Entry::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, FActivityNodePinTypes::Output_Success, TEXT("Out"));
}

const FSlateBrush* UActivityNode_Entry::GetNodeIcon() const
{
	return Super::GetNodeIcon();
}

void UActivityNode_Entry::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	AddContextMenuActionsServices(Menu, Context);
}

FText UActivityNode_Entry::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("EntryNodeTitle", "Entry");
}

FText UActivityNode_Entry::GetDescription() const
{
	return LOCTEXT("EntryNodeDescription", "The Entry point for this activity");
}

UObject* UActivityNode_Entry::GetNodeInstance() const
{
	return GetEditingActivity();
}

void UActivityNode_Entry::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (Pin == GetOutputPin())
	{
		if (Pin->LinkedTo.Num() > 0)
		{
			UActivityNode_Stage* StageNode = Cast<UActivityNode_Stage>(Pin->LinkedTo[0]->GetOwningNode());
			if (IsValid(StageNode))
			{
				GetEditingActivity()->InitialStage = StageNode->GetActivityStage();
			}
		}
		else
		{
			GetEditingActivity()->InitialStage = nullptr;
		}
	}
}

void UActivityNode_Entry::OnSubNodeAdded(UAIGraphNode* SubNode)
{
	Super::OnSubNodeAdded(SubNode);

	if (UActivityNode_Service* ServiceNode = Cast<UActivityNode_Service>(SubNode))
	{
		GetEditingActivity()->StageServices.AddUnique(ServiceNode->GetService());
	}
}

void UActivityNode_Entry::OnSubNodeRemoved(UAIGraphNode* SubNode)
{
	Super::OnSubNodeRemoved(SubNode);

	if (UActivityNode_Service* ServiceNode = Cast<UActivityNode_Service>(SubNode))
	{
		GetEditingActivity()->StageServices.Remove(ServiceNode->GetService());
	}
}

#undef LOCTEXT_NAMESPACE