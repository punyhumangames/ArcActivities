// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "DataModel/ArcActivityTask_Base.h"
#include "ArcActivityTypes.h"
#include "ArcActivityTask_StageService.generated.h"

class UArcActivityStage;



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ARCACTIVITIESPLUGIN_API UArcActivityTask_StageService : public UArcActivityTask_Base
{
	GENERATED_BODY()

public:
	friend class UArcActivityInstance;

	UArcActivityTask_StageService(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintNativeEvent)
	void StageBegun(EArcActivitySuccessState PreviousStageResult, UArcActivityStage* PreviousStage);
	virtual void StageBegun_Implementation(EArcActivitySuccessState PreviousStageResult, UArcActivityStage* PreviousStage) { };

	UFUNCTION(BlueprintNativeEvent)
	void StageEnded(EArcActivitySuccessState StageResult);
	virtual void  StageEnded_Implementation(EArcActivitySuccessState StageResult) { };
	
	UFUNCTION(BlueprintPure, Category = "Activity")
	UArcActivityStage* GetActivityStage() const { return OwningStage; }

private:
	
	TObjectPtr<UArcActivityStage> OwningStage;

};
