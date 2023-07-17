// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "Styling/SlateBrush.h"
#include "ActivityEditorTypes.h"
#include "AIGraphNode.h"

#include "ActivityNode_Base.generated.h"

class UArcActivity;
class UEdGraph_Activity;

UCLASS()
class UActivityNode_Base : public UAIGraphNode
{					
	GENERATED_BODY()
public:
	UActivityNode_Base(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void PostEditUndo() override;

	virtual void PostLoad() override;
#endif

	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;

	virtual void PostPlacedNewNode() override;

	virtual UObject* GetNodeInstance() const;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual const FSlateBrush* GetNodeIcon() const;
	virtual FSlateColor GetNodeBackgroundColor() const;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
	virtual FText GetDescription() const;
	virtual FText GetTooltipText() const override;

	UEdGraph_Activity* GetActivityGraph() const;
	UArcActivity* GetEditingActivity() const;

	virtual void OnSubNodeAdded(UAIGraphNode* SubNode) override;
	virtual void OnSubNodeRemoved(UAIGraphNode* SubNode) override;
	virtual void RemoveAllSubNodes() override;
	virtual int32 FindSubNodeDropIndex(UAIGraphNode* SubNode) const override;
	virtual void InsertSubNodeAt(UAIGraphNode* SubNode, int32 DropIndex) override;

		/** instance class */
	UPROPERTY()
	struct FArcGraphNodeClassData ArcClassData;

	UPROPERTY()
	TArray<UActivityNode_Base*> BottomSubNodes;
	UPROPERTY()
	TArray<UActivityNode_Base*> TopSubNodes;

	UPROPERTY()
	UClass* BottomSubNodeClass;
	UPROPERTY()
	UClass* TopSubNodeClass;

	void CreateAddServicesSubMenu(class FMenuBuilder& MenuBuilder, UEdGraph* Graph) const;
	void AddContextMenuActionsServices(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const;

	virtual bool RefreshNodeClass() override;
	virtual void UpdateNodeClassData() override;


	static void ArcUpdateNodeClassDataFrom(UClass* InstanceClass, FArcGraphNodeClassData& UpdatedData);

};
