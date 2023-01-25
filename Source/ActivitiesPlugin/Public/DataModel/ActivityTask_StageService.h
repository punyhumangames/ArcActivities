// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "DataModel/ActivityTask_Base.h"
#include "ArcActivityTypes.h"
#include "ActivityTask_StageService.generated.h"

class UActivityStage;



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ACTIVITIESPLUGIN_API UActivityTask_StageService : public UActivityTask_Base
{
	GENERATED_BODY()

public:
	friend class UArcActivityInstance;

	UActivityTask_StageService(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintNativeEvent)
	void StageBegun(EArcActivitySuccessState PreviousStageResult, UActivityStage* PreviousStage);
	virtual void StageBegun_Implementation(EArcActivitySuccessState PreviousStageResult, UActivityStage* PreviousStage) { };

	UFUNCTION(BlueprintNativeEvent)
	void StageEnded(EArcActivitySuccessState StageResult);
	virtual void  StageEnded_Implementation(EArcActivitySuccessState StageResult) { };
	
	UFUNCTION(BlueprintPure, Category = "Activity")
	UActivityStage* GetActivityStage() const { return OwningStage; }

private:
	
	TObjectPtr<UActivityStage> OwningStage;

};
