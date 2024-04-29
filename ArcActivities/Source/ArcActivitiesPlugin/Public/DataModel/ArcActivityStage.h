// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info

#pragma once

#include "CoreMinimal.h"
#include "ArcActivityTypes.h"
#include "ArcActivityStage.generated.h"

class UArcActivity;
class UArcActivityObjective;
class UArcActivityTask_StageService;
class UArcActivityUIObject;

/**
 * 
 */
UCLASS(BlueprintType)
class ARCACTIVITIESPLUGIN_API UArcActivityStage : public UObject
{
	GENERATED_BODY()
public:
	UArcActivityStage(const FObjectInitializer& ObjectInitializer);

	UArcActivity* GetOwningActivity();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Activity")
	FText StageName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	FText StageDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activity")
	EArcActivityCompletionMode CompletionMode;

	//How long to delay this stage's completion.
	UPROPERTY(EditAnywhere, Category="Activity")
	float StageCompletionDelay;

	UPROPERTY(VisibleAnywhere, Category="Activity")
	UArcActivityStage* NextStage_Success;
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UArcActivityStage* NextStage_Failure;

	UPROPERTY(VisibleAnywhere, Category="Activity")
	TArray<UArcActivityObjective*> Objectives;

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UArcActivityTask_StageService*> StageServices;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI", Instanced)
	TObjectPtr<UArcActivityUIObject> UIData;
	
};
