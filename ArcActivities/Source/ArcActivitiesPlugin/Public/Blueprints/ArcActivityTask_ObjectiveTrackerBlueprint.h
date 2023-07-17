// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "ArcActivityTask_ObjectiveTrackerBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivityTask_ObjectiveTrackerBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UArcActivityTask_ObjectiveTrackerBlueprint()
	{
		ParentClass = UArcActivityTask_ObjectiveTracker::StaticClass();
		//We must make sure the GeneratedClass is generated after the blueprint is loaded
		bRecompileOnLoad = true;
	}

#if WITH_EDITOR
	virtual UClass* GetBlueprintClass() const override { return UArcActivityTask_ObjectiveTracker::StaticClass(); }
#endif
};
