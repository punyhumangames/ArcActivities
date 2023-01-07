// 2017-2019 Puny Human

#include "ActivityNode_Base.h"
#include "EdGraph/EdGraph.h"
#include "Graph/EdGraph_Activity.h"
#include "Engine/Blueprint.h"
#include "BlueprintNodeHelpers.h"

#include "EdGraph/EdGraph.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SGraphEditorActionMenuAI.h"

#include "ToolMenus.h"

#include "Graph/EdGraphSchema_Activity.h"

#define LOCTEXT_NAMESPACE "ActivityNode"

UActivityNode_Base::UActivityNode_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsReadOnly = false;
}
#if WITH_EDITOR
void UActivityNode_Base::PostEditUndo()
{
	Super::PostEditUndo();

	if (UActivityNode_Base* MyParentNode = Cast<UActivityNode_Base>(ParentNode))
	{
		const bool bTopSubNode = GetClass()->IsChildOf(MyParentNode->TopSubNodeClass);
		const bool bBottomSubNode = GetClass()->IsChildOf(MyParentNode->BottomSubNodeClass);

		if (bTopSubNode)
		{
			MyParentNode->TopSubNodes.AddUnique(this);
		}
		if (bBottomSubNode)
		{
			MyParentNode->BottomSubNodes.AddUnique(this);
		}
	}
}

void UActivityNode_Base::PostLoad()
{
	Super::PostLoad();

	for (auto ChildNode : SubNodes)
	{
		ChildNode->ParentNode = this;
	}
}

#endif

bool UActivityNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UEdGraphSchema_Activity::StaticClass());
}

UObject* UActivityNode_Base::GetNodeInstance() const
{
	return NodeInstance;
}

FText UActivityNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return Super::GetNodeTitle(TitleType);
}

const FSlateBrush* UActivityNode_Base::GetNodeIcon() const
{
	return FAppStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
}

FSlateColor UActivityNode_Base::GetNodeBackgroundColor() const
{
	return FLinearColor(0.08f, 0.08f, 0.08f);
}

FLinearColor UActivityNode_Base::GetNodeTitleColor() const
{
	return FLinearColor(0.08f, 0.08f, 0.08f);
} 

FText UActivityNode_Base::GetPinDisplayName(const UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinCategory == FActivityNodePinTypes::Output_Success)
	{
		return LOCTEXT("OutputSuccessPin", "Success");
	}
	if (Pin->PinType.PinCategory == FActivityNodePinTypes::Output_Fail)
	{
		return LOCTEXT("OutputFailPin", "Failure");
	}
	if (Pin->PinType.PinCategory == FActivityNodePinTypes::Input)
	{
		return LOCTEXT("InputPin", "Input");
	}
	return LOCTEXT("UnknownPin", "Unknown");
}

FText UActivityNode_Base::GetDescription() const
{
	return LOCTEXT("NoDescription", "No Description Set");
}

UEdGraph_Activity* UActivityNode_Base::GetActivityGraph() const
{
	return CastChecked<UEdGraph_Activity>(GetGraph());
}

UActivity* UActivityNode_Base::GetEditingActivity() const
{
	if (UEdGraph_Activity* ActivityGraph = GetActivityGraph())
	{
		return ActivityGraph->GetActivity();
	}

	return nullptr;
}  	

void UActivityNode_Base::OnSubNodeAdded(UAIGraphNode* SubNode)
{
	if(UActivityNode_Base* ActivitySubNode = Cast<UActivityNode_Base>(SubNode))
	{
		//Determine if this is a top or bottom subnode
		const bool bTopSubNode = SubNode->GetClass()->IsChildOf(TopSubNodeClass);
		const bool bBottomSubNode = SubNode->GetClass()->IsChildOf(BottomSubNodeClass);		 

		if (bTopSubNode)
		{
			TopSubNodes.Add(ActivitySubNode);
		}
		else if (bBottomSubNode)
		{
			BottomSubNodes.Add(ActivitySubNode);
		}
	}	
}

void UActivityNode_Base::OnSubNodeRemoved(UAIGraphNode* SubNode)
{
	if (UActivityNode_Base* ActivitySubNode = Cast<UActivityNode_Base>(SubNode))
	{
		const int32 TopIdx = TopSubNodes.IndexOfByKey(ActivitySubNode);
		const int32 BottomIdx = BottomSubNodes.IndexOfByKey(ActivitySubNode);

		if (TopIdx >= 0)
		{
			TopSubNodes.RemoveAt(TopIdx);
		}
		if (BottomIdx >= 0)
		{
			BottomSubNodes.RemoveAt(BottomIdx);
		}
	}
}

void UActivityNode_Base::RemoveAllSubNodes()
{
	Super::RemoveAllSubNodes();

	TopSubNodes.Empty();
	BottomSubNodes.Empty();
}

int32 UActivityNode_Base::FindSubNodeDropIndex(UAIGraphNode* SubNode) const
{	   	
	const int32 SubIdx = SubNodes.IndexOfByKey(SubNode) + 1;
	const int32 TopIdx = TopSubNodes.IndexOfByKey(SubNode) + 1;
	const int32 BottomIdx = BottomSubNodes.IndexOfByKey(SubNode) + 1;

	const int32 CombinedIdx = (SubIdx & 0xff) | ((TopIdx & 0xff) << 8) | ((BottomIdx & 0xff) << 16);
	return CombinedIdx;	  	
}

void UActivityNode_Base::InsertSubNodeAt(UAIGraphNode* SubNode, int32 DropIndex)
{
	if (UActivityNode_Base* ActivitySubNode = Cast<UActivityNode_Base>(SubNode))
	{
		const int32 SubIdx = (DropIndex & 0xff) - 1;
		const int32 TopIdx = ((DropIndex >> 8) & 0xff) - 1;
		const int32 BottomIdx = ((DropIndex >> 16) & 0xff) - 1;

		const bool bTopSubNode = SubNode->GetClass()->IsChildOf(TopSubNodeClass);
		const bool bBottomSubNode = SubNode->GetClass()->IsChildOf(BottomSubNodeClass);

		if (SubIdx >= 0)
		{
			SubNodes.Insert(ActivitySubNode, SubIdx);
		}
		else
		{
			SubNodes.Add(ActivitySubNode);
		}

		if (bTopSubNode)
		{
			if (TopIdx >= 0)
			{
				TopSubNodes.Insert(ActivitySubNode, TopIdx);
			}
			else
			{
				TopSubNodes.Add(ActivitySubNode);
			}
		}

		if (bBottomSubNode)
		{
			if (BottomIdx >= 0)
			{
				BottomSubNodes.Insert(ActivitySubNode, BottomIdx);
			}
			else
			{
				BottomSubNodes.Add(ActivitySubNode);
			}
		}

	}
}

void UActivityNode_Base::CreateAddServicesSubMenu(class FMenuBuilder& MenuBuilder, UEdGraph* Graph) const
{
	TSharedRef<SGraphEditorActionMenuAI> Menu =
		SNew(SGraphEditorActionMenuAI)
		.GraphObj(Graph)
		.GraphNode((UActivityNode_Base*)this)
		.SubNodeFlags((uint32)EActivitySubnodeType::Service)
		.AutoExpandActionMenu(true);

	MenuBuilder.AddWidget(Menu, FText(), true);
}


void UActivityNode_Base::AddContextMenuActionsServices(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection("Services");
	Section.AddSubMenu("AddService",
		LOCTEXT("AddService", "Add Service..."),
		LOCTEXT("AddServiceTooltip", "Adds new service as a subnode"),
		FNewMenuDelegate::CreateUObject(this, &UActivityNode_Base::CreateAddServicesSubMenu, (UEdGraph*)Context->Graph));
}

#undef LOCTEXT_NAMESPACE