// 2017-2019 Puny Human

#pragma once

#include "EdGraph/EdGraph.h"
#include "AIGraph.h"
#include "EdGraph_Activity.generated.h"

class UActivity;

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

	void InitializeGraph(UActivity* DataAsset);
	virtual void OnCreated() override;
	void RefreshNodeSelection(UEdGraphNode* Node);

	virtual void OnSubNodeDropped();

	virtual UActivity* GetActivity();

};