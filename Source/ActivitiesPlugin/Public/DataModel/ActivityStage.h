// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "ActivityStage.generated.h"

UENUM(BlueprintType)
enum class EActivityStageProgression : uint8
{
	Success,
	Failed,
};

class UActivityObjective;
class UActivityTask_StageService;

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UActivityStage : public UObject
{
	GENERATED_BODY()
public:
	UActivityStage(const FObjectInitializer& ObjectInitializer);

	UActivity* GetOwningActivity();

	UPROPERTY(EditAnywhere, Category="Activity")
	FText StageName;

	UPROPERTY(EditAnywhere, Category = "Activity")
	FText StageDescription;

	UPROPERTY(VisibleAnywhere, Category="Activity")
	UActivityStage* NextStage_Success;
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UActivityStage* NextStage_Failure;

	UPROPERTY(VisibleAnywhere, Category="Activity")
	TArray<UActivityObjective*> Objectives;

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UActivityTask_StageService*> StageServices;
	
};
