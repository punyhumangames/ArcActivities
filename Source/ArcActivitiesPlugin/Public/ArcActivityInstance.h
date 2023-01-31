// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.h"
#include "ArcActivityInstance.generated.h"

class UArcActivity;
class UArcActivityPlayerComponent;
class UArcActivityStage;
class UArcActivityTask_StageService;
class UArcActivityTask_ObjectiveTracker;

DECLARE_MULTICAST_DELEGATE_TwoParams(FArcActivityDelegateEnded, UArcActivityInstance* /* Instnace */, bool /* bWasCancelled */)

/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivityInstance : public UObject
{
	GENERATED_BODY()
public:
	friend class UArcActivityWorldSubsystem;

	virtual class UWorld* GetWorld() const override { return World.Get(); }

	bool IsActive() const;
	void EndActivity(bool bWasCancelled = false);

	UFUNCTION(BlueprintCallable)
	void AddPlayerToActivity(UArcActivityPlayerComponent* Player);
	UFUNCTION(BlueprintCallable)
	void RemovePlayerFromActivity(UArcActivityPlayerComponent* Player);
	bool TryProgressStage();



	using ConstForEachStageServiceFunc = TFunctionRef<void(const UArcActivityTask_StageService*)>;
	void ForEachGlobalStageService(ConstForEachStageServiceFunc Func) const;
	void ForEachCurrentStageService(ConstForEachStageServiceFunc Func) const;
	void ForEachStageService(ConstForEachStageServiceFunc Func) const 
	{
		ForEachGlobalStageService(Func);
		ForEachCurrentStageService(Func);
	}

	using ForEachStageServiceFunc = TFunctionRef<void(UArcActivityTask_StageService*)>;
	void ForEachGlobalStageService_Mutable(ForEachStageServiceFunc Func) const;
	void ForEachCurrentStageService_Mutable(ForEachStageServiceFunc Func) const;
	void ForEachStageService_Mutable(ForEachStageServiceFunc Func) const
	{
		ForEachGlobalStageService_Mutable(Func);
		ForEachCurrentStageService_Mutable(Func);
	}

	using ConstForEachObjectiveTrackerFunc = TFunctionRef<void(const UArcActivityTask_ObjectiveTracker*)>;
	void ForEachObjectiveTracker(ConstForEachObjectiveTrackerFunc Func) const;

	using ForEachObjectiveTrackerFunc = TFunctionRef<void(UArcActivityTask_ObjectiveTracker*)>;
	void ForEachObjectiveTracker_Mutable(ForEachObjectiveTrackerFunc Func) const;

	UFUNCTION(BlueprintPure, Category="Activity")
	EArcActivitySuccessState GetActivityState() const { return ActivityState; }

private:
	//BEGIN ACCESSED BY ARC ACTIVITY GI SUBSYSTEM
	bool InitActivityGraph(UArcActivity* Graph, const FGameplayTagContainer& Tags);

	void ProgressStage_Internal(EArcActivitySuccessState Transition);

	void EnterStage_Internal(UArcActivityStage* Stage);
	void ExitStage_Internal(UArcActivityStage* Stage);

	void TrackerUpdated_Internal(UArcActivityTask_ObjectiveTracker* Tracker);

	FArcActivityDelegateEnded OnActivityEnded;

	
	EArcActivitySuccessState ActivityState;

	UPROPERTY()
	TObjectPtr<UArcActivity> ActivityGraph;

	UPROPERTY()
	FGameplayTagContainer ActivityTags;

	UPROPERTY()
	TArray<TObjectPtr<UArcActivityPlayerComponent>> PlayersInActivty;

	UPROPERTY()
	TObjectPtr<UArcActivityStage> CurrentStage;

	UPROPERTY()
	TArray<TObjectPtr<UArcActivityTask_StageService>> CurrentGlobalStageServices;

	UPROPERTY()
	TArray< TObjectPtr<UArcActivityTask_StageService>> CurrentStageServices;

	UPROPERTY()
	TArray<TObjectPtr< UArcActivityTask_ObjectiveTracker>> CurrentObjectiveTrackers;

	TWeakObjectPtr<UWorld> World;
	
};
