// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "DataModel/ObjectiveTracker.h"
#include "ObjectiveTrackerBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UObjectiveTrackerBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UObjectiveTrackerBlueprint()
	{
		ParentClass = UActivityTask_ObjectiveTracker::StaticClass();
		//We must make sure the GeneratedClass is generated after the blueprint is loaded
		bRecompileOnLoad = true;
	}

#if WITH_EDITOR
	virtual UClass* GetBlueprintClass() const override { return UActivityTask_ObjectiveTracker::StaticClass(); }
#endif
};
