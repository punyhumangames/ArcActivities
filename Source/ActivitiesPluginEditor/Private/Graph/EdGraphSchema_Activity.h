// 2017-2019 Puny Human

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "ActivityEditorTypes.h"
#include "AIGraphSchema.h"
#include "EdGraphSchema_Activity.generated.h"


class UActivityNode_Base;

UCLASS()
class UEdGraphSchema_Activity : public UAIGraphSchema
{
	GENERATED_BODY()
public:
	UEdGraphSchema_Activity(const FObjectInitializer& ObjectInitializer);
	   

	// Begin EdGraphSchema interface
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	// End EdGraphSchema interface

	virtual void GetSubNodeClasses(int32 SubNodeFlags, TArray<FGraphNodeClassData>& ClassData, UClass*& GraphNodeClass) const;

};

