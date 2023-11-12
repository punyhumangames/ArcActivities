// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#include "ActivitiesPluginEditor.h"

#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"
#include "EdGraphUtilities.h"
#include "IAssetTools.h"

const float GRID_SNAP = 16.0f; 

bool FNodeVisitorCycleChecker::CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
{
	VisitedNodes.Add(EndNode);
	return TraverseInputNodesToRoot(StartNode);
}

bool FNodeVisitorCycleChecker::TraverseInputNodesToRoot(UEdGraphNode* Node)
{
	VisitedNodes.Add(Node);

	// Follow every input pin until we cant any more ('root') or we reach a node we have seen (cycle)
	for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* MyPin = Node->Pins[PinIndex];

		if (MyPin->Direction == EGPD_Input)
		{
			for (int32 LinkedPinIndex = 0; LinkedPinIndex < MyPin->LinkedTo.Num(); ++LinkedPinIndex)
			{
				UEdGraphPin* OtherPin = MyPin->LinkedTo[LinkedPinIndex];
				if (OtherPin)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if (VisitedNodes.Contains(OtherNode))
					{
						return false;
					}
					else
					{
						return TraverseInputNodesToRoot(OtherNode);
					}
				}
			}
		}
	}

	return true;
}