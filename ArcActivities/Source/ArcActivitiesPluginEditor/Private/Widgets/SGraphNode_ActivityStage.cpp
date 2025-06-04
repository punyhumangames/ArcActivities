// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#include "SGraphNode_ActivityStage.h"

#include "Types/SlateStructs.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SToolTip.h"
#include "GraphEditorSettings.h"
#include "SGraphPanel.h"
#include "SCommentBubble.h"
#include "SGraphPreviewer.h"
#include "NodeFactory.h"

#include "IDocumentation.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SLevelOfDetailBranchNode.h"

#include "Graph/EdGraph_Activity.h"
#include "Graph/Nodes/ActivityNode_Base.h"
#include "Graph/Nodes/ActivityNode_Entry.h"

#define LOCTEXT_NAMESPACE "ActivityEditor"


class SActivityTreePin : public SGraphPinAI
{
public:
	SLATE_BEGIN_ARGS(SActivityTreePin) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
		{
			this->SetCursor(EMouseCursor::Default);

			bShowLabel = true;

			GraphPinObj = InPin;
			check(GraphPinObj != NULL);

			const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
			check(Schema);

			TSharedRef<SWidget> LabelWidget = SNew(STextBlock)
				.Text(this, &SActivityTreePin::GetPinLabel)
				//.TextStyle(FEditorStyle::Get(), InLabelStyle)
				.Visibility(this, &SActivityTreePin::GetPinLabelVisibility)
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(this, &SActivityTreePin::GetPinTextColor);

			TSharedPtr<SHorizontalBox> PinContent;

			const bool bIsInput = (GetDirection() == EGPD_Input);

			

			if (!bIsInput)
			{
				FullPinHorizontalRowWidget = PinContent = SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					.Padding(FMargin(0))
					[
						LabelWidget
					];
			}
			else
			{
				FullPinHorizontalRowWidget = PinContent = SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					.Padding(FMargin(0));
			}
			

			SBorder::Construct(SBorder::FArguments()
				.BorderImage(this, &SActivityTreePin::GetPinBorder)
				.BorderBackgroundColor(this, &SActivityTreePin::GetPinColor)
				.OnMouseButtonDown(this, &SActivityTreePin::OnPinMouseDown)
				.Cursor(this, &SActivityTreePin::GetPinCursor)
				.Padding(FMargin(10.0f))
				[
					PinContent.ToSharedRef()
				]
			);
			
		}
protected:
	/** @return The color that we should use to draw this pin */
	virtual FSlateColor GetPinColor() const override
	{
		return GraphPinObj->GetSchema()->GetPinTypeColor(GraphPinObj->PinType);
	}
};

void SGraphNode_Activity::Construct(const FArguments& InArgs, UActivityNode_Base* InNode)
{

	SGraphNodeAI::Construct(SGraphNodeAI::FArguments(), InNode);
}

void SGraphNode_Activity::UpdateGraphNode()
{
	bDragMarkerVisible = false;
	InputPins.Empty();
	OutputPins.Empty();

	if (TopSubnodesBox.IsValid())
	{
		TopSubnodesBox->ClearChildren();
	}
	else
	{
		SAssignNew(TopSubnodesBox, SVerticalBox);
	}

	if (BottomSubnodesBox.IsValid())
	{
		BottomSubnodesBox->ClearChildren();
	}
	else
	{
		SAssignNew(BottomSubnodesBox, SVerticalBox);
	}

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	TopSubnodeWidgets.Reset();
	BottomSubnodeWidgets.Reset();
	SubNodes.Reset();
	OutputPinBox.Reset();

	UActivityNode_Base* ActivityNode = Cast<UActivityNode_Base>(GraphNode);

	if (IsValid(ActivityNode))
	{
		for (UActivityNode_Base* SubNode : ActivityNode->TopSubNodes)
		{
			TSharedPtr<SGraphNode> NewNode = FNodeFactory::CreateNodeWidget(SubNode);
			if (OwnerGraphPanelPtr.IsValid())
			{
				NewNode->SetOwner(OwnerGraphPanelPtr.Pin().ToSharedRef());
				OwnerGraphPanelPtr.Pin()->AttachGraphEvents(NewNode);
			}
			AddTopSubnode(NewNode);
			NewNode->UpdateGraphNode();
		}

		for (UActivityNode_Base* SubNode : ActivityNode->BottomSubNodes)
		{
			TSharedPtr<SGraphNode> NewNode = FNodeFactory::CreateNodeWidget(SubNode);
			if (OwnerGraphPanelPtr.IsValid())
			{
				NewNode->SetOwner(OwnerGraphPanelPtr.Pin().ToSharedRef());
				OwnerGraphPanelPtr.Pin()->AttachGraphEvents(NewNode);
			}
			AddBottomSubnode(NewNode);
			NewNode->UpdateGraphNode();
		}
	}						   								

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<STextBlock> DescriptionText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
	auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
		return FMath::Max(75.0f, DesiredWidth);
	};
	auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
		return FMath::Max(22.0f, DesiredHeight);
	};

	const FMargin NodePadding = FMargin(8.0f);
	   
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			.BorderBackgroundColor(this, &SGraphNode_Activity::GetBorderBackgroundColor)
			.OnMouseButtonDown(this, &SGraphNode_Activity::OnMouseDown)
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					// INPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+ SVerticalBox::Slot()
						.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
						[
							SNew(SVerticalBox)							
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(NodeBody, SBorder)
								.BorderImage(FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
								.BorderBackgroundColor(this, &SGraphNode_Activity::GetBackgroundColor)
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Center)
								.Visibility(EVisibility::SelfHitTestInvisible)
								[
									SNew(SOverlay)
									+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SGraphNode_Activity::GetErrorColor)
											.ToolTipText(this, &SGraphNode_Activity::GetErrorMsgToolTip)
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SLevelOfDetailBranchNode)
											.UseLowDetailSlot(this, &SGraphNode_Activity::UseLowDetailNodeTitles)
											.LowDetail()
											[
												SNew(SBox)
												.WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
												.HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
											]
											.HighDetail()
											[
												SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.AutoWidth()
												.VAlign(VAlign_Center)
												[
													SNew(SImage)
													.Image(this, &SGraphNode_Activity::GetNameIcon)
												]
												+ SHorizontalBox::Slot()
												.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
												[
													SNew(SVerticalBox)
													+ SVerticalBox::Slot()
													.AutoHeight()
													[
														SAssignNew(InlineEditableText, SInlineEditableTextBlock)
														.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
														.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
														.OnVerifyTextChanged(this, &SGraphNode_Activity::OnVerifyNameTextChanged)
														.OnTextCommitted(this, &SGraphNode_Activity::OnNameTextCommited)
														.IsReadOnly(this, &SGraphNode_Activity::IsNameReadOnly)
														.IsSelected(this, &SGraphNode_Activity::IsSelectedExclusively)
													]
													+ SVerticalBox::Slot()
													.AutoHeight()
													[
														NodeTitle.ToSharedRef()
													]
												]
											]
										]
									]
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										// DESCRIPTION MESSAGE
										SAssignNew(DescriptionText, STextBlock)
										.Visibility(this, &SGraphNode_Activity::GetDescriptionVisibility)
										.Text(this, &SGraphNode_Activity::GetDescription)
									]
								]			  								
							]
						]
					//OBJECTIVES NODES
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMargin(10.0f, 0, 0, 0))
						[
							BottomSubnodesBox.ToSharedRef()
						]
						//SERVICES NODES
						+ SVerticalBox::Slot()
							.AutoHeight()
							[
								TopSubnodesBox.ToSharedRef()
							]
					]

					// OUTPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Fill)
							.Padding(20.0f, 0.0f)
							.FillHeight(1.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]
				]

				// Drag marker overlay
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SNew(SBorder)
					//.BorderBackgroundColor(SGraphNode_Activity::Action::DragMarker)
					//.ColorAndOpacity(SGraphNode_Activity::Action::DragMarker)
					.BorderImage(FAppStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
					.Visibility(this, &SGraphNode_Activity::GetDragOverMarkerVisibility)
					[
						SNew(SBox)
						.HeightOverride(4)
					]
				]

				// Blueprint indicator overlay
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Blueprint")))
					.Visibility(this, &SGraphNode_Activity::GetBlueprintIconVisibility)
				]
			]
		];
	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SGraphNode_Activity::CreatePinWidgets()
{
	UActivityNode_Base* ActivityNode = CastChecked<UActivityNode_Base>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < ActivityNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = ActivityNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SActivityTreePin, MyPin)
				.ToolTipText(this, &SGraphNode_Activity::GetPinTooltip, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SGraphNode_Activity::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
	
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(0.4f)
			.Padding(0, 0, 20.0f, 0)
			[
				PinToAdd 				
			];

	
		OutputPins.Add(PinToAdd);
	}
}

TSharedPtr<SToolTip> SGraphNode_Activity::GetComplexTooltip()
{
	return SGraphNode::GetComplexTooltip();
}

void SGraphNode_Activity::GetOverlayBrushes(bool bSelected, const FVector2D WidgetSize, TArray<FOverlayBrushInfo>& Brushes) const
{
	
}

TArray<FOverlayWidgetInfo> SGraphNode_Activity::GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const
{
	//TODO: This for StageServices, Objectives, and Objective Trackers
	return TArray<FOverlayWidgetInfo>();
}

TSharedRef<SGraphNode> SGraphNode_Activity::GetNodeUnderMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<SGraphNode> SubNode = ArcGetSubNodeUnderCursor(MyGeometry, MouseEvent);
	return SubNode.IsValid() ? SubNode.ToSharedRef() : StaticCastSharedRef<SGraphNode>(AsShared());
}



TSharedPtr<SGraphNode> SGraphNode_Activity::ArcGetSubNodeUnderCursor(const FGeometry& WidgetGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<SGraphNode> ResultNode;

	// We just need to find the one WidgetToFind among our descendants.
	TSet< TSharedRef<SWidget> > SubWidgetsSet;
	AddSubnodesToSetRecursively(SubWidgetsSet);

	TMap<TSharedRef<SWidget>, FArrangedWidget> Result;
	FindChildGeometries(WidgetGeometry, SubWidgetsSet, Result);

	if (Result.Num() > 0)
	{
		FArrangedChildren ArrangedChildren(EVisibility::Visible);
		Result.GenerateValueArray(ArrangedChildren.GetInternalArray());

		const int32 HoveredIndex = SWidget::FindChildUnderMouse(ArrangedChildren, MouseEvent);
		if (HoveredIndex != INDEX_NONE)
		{
			ResultNode = StaticCastSharedRef<SGraphNode>(ArrangedChildren[HoveredIndex].Widget);
		}
	}

	return ResultNode;
}

void SGraphNode_Activity::AddSubnodesToSetRecursively(TSet<TSharedRef<SWidget>>& Set)
{
	for (int32 i = 0; i < SubNodes.Num(); i++)
	{
		TSharedPtr<SWidget> SubNodeWidget = SubNodes[i];
		Set.Add(SubNodes[i].ToSharedRef());
		if (TSharedPtr<SGraphNode_Activity> ActivityWidget = StaticCastSharedPtr< SGraphNode_Activity>(SubNodeWidget))
		{
			ActivityWidget->AddSubnodesToSetRecursively(Set);
		}
	}
}

void SGraphNode_Activity::MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	SGraphNodeAI::MoveTo(NewPosition, NodeFilter, bMarkDirty);

	// keep node order (defined by linked pins) up to date with actual positions
	// this function will keep spamming on every mouse move update
	UActivityNode_Base* ActivityNode = Cast<UActivityNode_Base>(GraphNode);
	if (ActivityNode && !ActivityNode->IsSubNode())
	{
		UEdGraph_Activity* ActivityGraph = ActivityNode->GetActivityGraph();
		if (ActivityGraph)
		{
			for (int32 Idx = 0; Idx < ActivityNode->Pins.Num(); Idx++)
			{
				UEdGraphPin* Pin = ActivityNode->Pins[Idx];
				if (Pin && Pin->Direction == EGPD_Input && Pin->LinkedTo.Num() == 1)
				{
					UEdGraphPin* ParentPin = Pin->LinkedTo[0];
					if (ParentPin)
					{
						//ActivityGraph->RebuildChildOrder(ParentPin->GetOwningNode());
					}
				}
			}
		}
	}
}

FReply SGraphNode_Activity::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return SGraphNode::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}

void SGraphNode_Activity::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SGraphNode::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

}

void SGraphNode_Activity::AddTopSubnode(TSharedPtr<SGraphNode> TopSubnodeWidget)
{
	TopSubnodesBox->AddSlot().AutoHeight()
		[
			TopSubnodeWidget.ToSharedRef()
		];

	TopSubnodeWidgets.Add(TopSubnodeWidget);
	AddSubNode(TopSubnodeWidget);
}

void SGraphNode_Activity::AddBottomSubnode(TSharedPtr<SGraphNode> BottomSubnodeWidget)
{
	BottomSubnodesBox->AddSlot().AutoHeight()
		[
			BottomSubnodeWidget.ToSharedRef()
		];

	BottomSubnodeWidgets.Add(BottomSubnodeWidget);
	AddSubNode(BottomSubnodeWidget);
}

FSlateColor SGraphNode_Activity::GetBorderBackgroundColor() const
{
	return FLinearColor(0.2f, 0.2f, 0.2f, 0.2f);
}

FSlateColor SGraphNode_Activity::GetBackgroundColor() const
{
	return FLinearColor(0.2f, 0.2f, 0.2f, 0.2f);
}

const FSlateBrush* SGraphNode_Activity::GetNameIcon() const
{
	return FAppStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

EVisibility SGraphNode_Activity::GetBlueprintIconVisibility() const
{
	UActivityNode_Base* ActivityGraphNode = Cast<UActivityNode_Base>(GraphNode);
	const bool bCanShowIcon = (ActivityGraphNode != nullptr && ActivityGraphNode->UsesBlueprint());

	// LOD this out once things get too small
	TSharedPtr<SGraphPanel> MyOwnerPanel = GetOwnerPanel();
	return (bCanShowIcon && (!MyOwnerPanel.IsValid() || MyOwnerPanel->GetCurrentLOD() > EGraphRenderingLOD::LowDetail)) ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SGraphNode_Activity::GetPinTooltip(UEdGraphPin* GraphPinObj) const
{
	FText HoverText = FText::GetEmpty();

	check(GraphPinObj != nullptr);
	UEdGraphNode* OwningGraphNode = GraphPinObj->GetOwningNode();
	if (OwningGraphNode != nullptr)
	{
		FString HoverStr;
		OwningGraphNode->GetPinHoverText(*GraphPinObj, /*out*/HoverStr);
		if (!HoverStr.IsEmpty())
		{
			HoverText = FText::FromString(HoverStr);
		}
	}

	return HoverText;
}

