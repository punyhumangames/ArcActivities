// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "DataModel/ArcActivityTask_StageService.h"
#include "ArcActivityTask_StageServiceBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivityTask_StageServiceBlueprint : public UBlueprint
{
	GENERATED_BODY()
public:
	UArcActivityTask_StageServiceBlueprint()
	{
		ParentClass = UArcActivityTask_StageService::StaticClass();
		//We must make sure the GeneratedClass is generated after the blueprint is loaded
		bRecompileOnLoad = true;
	}

#if WITH_EDITOR
	virtual UClass* GetBlueprintClass() const override { return UArcActivityTask_StageService::StaticClass(); }
#endif
};
