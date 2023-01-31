// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "ArcActivityTypes.h"
#include "ArcActivityStage.generated.h"


class UArcActivityObjective;
class UArcActivityTask_StageService;

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivityStage : public UObject
{
	GENERATED_BODY()
public:
	UArcActivityStage(const FObjectInitializer& ObjectInitializer);

	UArcActivity* GetOwningActivity();

	UPROPERTY(EditAnywhere, Category="Activity")
	FText StageName;

	UPROPERTY(EditAnywhere, Category = "Activity")
	FText StageDescription;

	UPROPERTY(EditAnywhere, Category = "Activity")
		EArcActivityCompletionMode CompletionMode;


	UPROPERTY(VisibleAnywhere, Category="Activity")
	UArcActivityStage* NextStage_Success;
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UArcActivityStage* NextStage_Failure;

	UPROPERTY(VisibleAnywhere, Category="Activity")
	TArray<UArcActivityObjective*> Objectives;

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UArcActivityTask_StageService*> StageServices;
	
};