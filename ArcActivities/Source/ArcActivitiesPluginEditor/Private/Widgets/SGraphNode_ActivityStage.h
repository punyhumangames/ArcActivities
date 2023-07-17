// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Styling/SlateColor.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SNodePanel.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "SGraphNodeAI.h"


class SHorizontalBox;
class SToolTip;
class SVerticalBox;
class UActivityNode_Base;

struct FNodeBounds
{
	FVector2D Position;
	FVector2D Size;

	FNodeBounds(FVector2D InPos, FVector2D InSize)
	{
		Position = InPos;
		Size = InSize;
	}
};

class SGraphNode_Activity : public SGraphNodeAI
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_Activity) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UActivityNode_Base* InNode);

	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SToolTip> GetComplexTooltip() override;
	virtual void GetOverlayBrushes(bool bSelected, const FVector2D WidgetSize, TArray<FOverlayBrushInfo>& Brushes) const override;
	virtual TArray<FOverlayWidgetInfo> GetOverlayWidgets(bool bSelected, const FVector2D& WidgetSize) const override;
	virtual TSharedRef<SGraphNode> GetNodeUnderMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty) override;
	// End of SGraphNode interface

	/** handle double click */
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

	/**
	 * Ticks this widget.  Override in derived classes, but always call the parent implementation.
	 *
	 * @param  AllottedGeometry The space allotted for this widget
	 * @param  InCurrentTime  Current absolute real time
	 * @param  InDeltaTime  Real time passed since last tick
	 */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void AddTopSubnode(TSharedPtr<SGraphNode> TopSubnodeWidget);

	void AddBottomSubnode(TSharedPtr<SGraphNode> BottomSubnodeWidget);
			   
	FVector2D GetCachedPosition() const { return CachedPosition; }

protected:

	/** height offsets for search triggers */
	TArray<FNodeBounds> TriggerOffsets;

	/** cached draw position */
	FVector2D CachedPosition;

	TArray< TSharedPtr<SGraphNode> > TopSubnodeWidgets;
	TArray< TSharedPtr<SGraphNode> > BottomSubnodeWidgets;
	TSharedPtr<SVerticalBox> TopSubnodesBox;
	TSharedPtr<SVerticalBox> BottomSubnodesBox;
	TSharedPtr<SHorizontalBox> OutputPinBox;
				   

	/** The node body widget, cached here so we can determine its size when we want ot position our overlays */
	TSharedPtr<SBorder> NodeBody;

	FSlateColor GetBorderBackgroundColor() const;
	FSlateColor GetBackgroundColor() const;

	virtual const FSlateBrush* GetNameIcon() const override;
	virtual EVisibility GetBlueprintIconVisibility() const;
							  
	FText GetPinTooltip(UEdGraphPin* GraphPinObj) const;
};