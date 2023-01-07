// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "DataModel/ActivityTask_StageService.h"
#include "ActivityServiceBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UActivityServiceBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UActivityServiceBlueprint()
	{
		ParentClass = UActivityTask_StageService::StaticClass();
		//We must make sure the GeneratedClass is generated after the blueprint is loaded
		bRecompileOnLoad = true;
	}

#if WITH_EDITOR
	virtual UClass* GetBlueprintClass() const override { return UActivityTask_StageService::StaticClass(); }
#endif
};
