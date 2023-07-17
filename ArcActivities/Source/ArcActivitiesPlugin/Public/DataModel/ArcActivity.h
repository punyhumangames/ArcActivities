// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ArcActivity.generated.h"

class UArcActivityStage;
class UArcActivityTask_StageService;
class UEdGraph;

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivity : public UPrimaryDataAsset, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	UArcActivity(const FObjectInitializer& ObjectInitializer);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
	{
		TagContainer.AppendTags(Tags);
	}

	UPROPERTY(VisibleAnywhere, Category = "Activity")
	UArcActivityStage* InitialStage;

	UPROPERTY()
	TArray<UArcActivityStage*> AllStages;

	//Global Stage Services for this activity. 
	UPROPERTY(VisibleAnywhere, Category = "Activity")
	TArray<UArcActivityTask_StageService*> StageServices;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activity")
	FGameplayTagContainer Tags;

	//How long in seconds before moving to the next stage.  
	//If number is 0 or less, will instantly move stages
	UPROPERTY(EditAnywhere, Category = "Activity")
	float BaseStageProgressionTimer;

#if WITH_EDITORONLY_DATA
	//Graph Representation
	UPROPERTY()
	UEdGraph* EditorGraph;	  
#endif
};
