// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.h"
#include "ArcActivityInstance.generated.h"

class UActivity;
class UArcActivityPlayerComponent;
class UActivityStage;
class UActivityTask_StageService;
class UActivityTask_ObjectiveTracker;

DECLARE_MULTICAST_DELEGATE_TwoParams(FArcActivityDelegateEnded, UArcActivityInstance* /* Instnace */, bool /* bWasCancelled */)

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UArcActivityInstance : public UObject
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



	using ConstForEachStageServiceFunc = TFunctionRef<void(const UActivityTask_StageService*)>;
	void ForEachGlobalStageService(ConstForEachStageServiceFunc Func) const;
	void ForEachCurrentStageService(ConstForEachStageServiceFunc Func) const;
	void ForEachStageService(ConstForEachStageServiceFunc Func) const 
	{
		ForEachGlobalStageService(Func);
		ForEachCurrentStageService(Func);
	}

	using ForEachStageServiceFunc = TFunctionRef<void(UActivityTask_StageService*)>;
	void ForEachGlobalStageService_Mutable(ForEachStageServiceFunc Func) const;
	void ForEachCurrentStageService_Mutable(ForEachStageServiceFunc Func) const;
	void ForEachStageService_Mutable(ForEachStageServiceFunc Func) const
	{
		ForEachGlobalStageService_Mutable(Func);
		ForEachCurrentStageService_Mutable(Func);
	}

	using ConstForEachObjectiveTrackerFunc = TFunctionRef<void(const UActivityTask_ObjectiveTracker*)>;
	void ForEachObjectiveTracker(ConstForEachObjectiveTrackerFunc Func) const;

	using ForEachObjectiveTrackerFunc = TFunctionRef<void(UActivityTask_ObjectiveTracker*)>;
	void ForEachObjectiveTracker_Mutable(ForEachObjectiveTrackerFunc Func) const;

	UFUNCTION(BlueprintPure, Category="Activity")
	EArcActivitySuccessState GetActivityState() const { return ActivityState; }

private:
	//BEGIN ACCESSED BY ARC ACTIVITY GI SUBSYSTEM
	bool InitActivityGraph(UActivity* Graph, const FGameplayTagContainer& Tags);

	void ProgressStage_Internal(EArcActivitySuccessState Transition);

	void EnterStage_Internal(UActivityStage* Stage);
	void ExitStage_Internal(UActivityStage* Stage);

	void TrackerUpdated_Internal(UActivityTask_ObjectiveTracker* Tracker);

	FArcActivityDelegateEnded OnActivityEnded;

	
	EArcActivitySuccessState ActivityState;

	UPROPERTY()
	TObjectPtr<UActivity> ActivityGraph;

	UPROPERTY()
	FGameplayTagContainer ActivityTags;

	UPROPERTY()
	TArray<TObjectPtr<UArcActivityPlayerComponent>> PlayersInActivty;

	UPROPERTY()
	TObjectPtr<UActivityStage> CurrentStage;

	UPROPERTY()
	TArray<TObjectPtr<UActivityTask_StageService>> CurrentGlobalStageServices;

	UPROPERTY()
	TArray< TObjectPtr<UActivityTask_StageService>> CurrentStageServices;

	UPROPERTY()
	TArray<TObjectPtr< UActivityTask_ObjectiveTracker>> CurrentObjectiveTrackers;

	TWeakObjectPtr<UWorld> World;
	
};
