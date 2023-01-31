// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "EdGraph/EdGraph.h"
#include "AIGraph.h"
#include "EdGraph_Activity.generated.h"

class UArcActivity;

struct FActivityNodePinTypes
{
	static const FName Input;
	static const FName Output_Success;
	static const FName Output_Fail;
};

UCLASS()
class UEdGraph_Activity : public UAIGraph
{
    GENERATED_BODY()
public:
	UEdGraph_Activity(const FObjectInitializer& ObjectInitializer);

	void InitializeGraph(UArcActivity* DataAsset);
	virtual void OnCreated() override;
	void RefreshNodeSelection(UEdGraphNode* Node);

	virtual void OnSubNodeDropped();

	virtual UArcActivity* GetActivity();

};