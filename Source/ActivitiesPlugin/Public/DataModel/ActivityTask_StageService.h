// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "DataModel/ActivityTask_Base.h"
#include "ActivityStage.h"
#include "ActivityTask_StageService.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ACTIVITIESPLUGIN_API UActivityTask_StageService : public UActivityTask_Base
{
	GENERATED_BODY()

public:
	UActivityTask_StageService(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintNativeEvent)
		void StageBegun(EActivityStageProgression PreviousStage);
	virtual void StageBegun_Implementation(EActivityStageProgression PreviousStage);

	UFUNCTION(BlueprintNativeEvent)
	void StageEnded(EActivityStageProgression StageResult);
	virtual void  StageEnded_Implementation(EActivityStageProgression StageResult);
	
	
};
