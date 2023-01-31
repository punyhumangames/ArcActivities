// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcActivity.generated.h"

class UArcActivityStage;
class UArcActivityTask_StageService;
class UEdGraph;

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivity : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UArcActivity(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UArcActivityStage* InitialStage;

	UPROPERTY()
	TArray<UArcActivityStage*> AllStages;

	//Global Stage Services for this activity. 
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UArcActivityTask_StageService*> StageServices;

#if WITH_EDITORONLY_DATA
	//Graph Representation
	UPROPERTY()
	UEdGraph* EditorGraph;	  
#endif
};
