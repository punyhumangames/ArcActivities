// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Activity.generated.h"

class UActivityStage;
class UActivityTask_StageService;
class UEdGraph;

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UActivity : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UActivity(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UActivityStage* InitialStage;

	UPROPERTY()
	TArray<UActivityStage*> AllStages;

	//Global Stage Services for this activity. 
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UActivityTask_StageService*> StageServices;

#if WITH_EDITORONLY_DATA
	//Graph Representation
	UPROPERTY()
	UEdGraph* EditorGraph;	  
#endif
};
