// 2017-2019 Puny Human

#pragma once

#include "CoreMinimal.h"
#include "ActivityTask_Base.h"
#include "ObjectiveTracker.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class ACTIVITIESPLUGIN_API UActivityTask_ObjectiveTracker : public UActivityTask_Base
{
	GENERATED_BODY()
public:
	friend class UArcActivityInstance;

	UActivityTask_ObjectiveTracker(const FObjectInitializer& ObjectInitializer);

	UActivityObjective* GetOwningObjective() const;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeTracker();
	virtual void InitializeTracker_Implementation();

private:

	UPROPERTY()
	TObjectPtr<UActivityObjective> ObjectiveRef;

};
