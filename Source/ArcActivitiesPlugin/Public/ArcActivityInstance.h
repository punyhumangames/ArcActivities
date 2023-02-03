// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.h"
#include "ArcActivityWorldSubsystem.h"
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
UCLASS(BlueprintType)
class ARCACTIVITIESPLUGIN_API UArcActivityInstance : public UObject
{
	GENERATED_BODY()
public:
	friend class UArcActivityWorldSubsystem;

	virtual class UWorld* GetWorld() const override { return World.Get(); }

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool IsNameStableForNetworking() const override { return false; }
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsActive() const;
	void EndActivity(bool bWasCancelled = false);

	UFUNCTION(BlueprintCallable, Category="Activity")
	TArray<UArcActivityTask_ObjectiveTracker*> GetCurrentObjectiveTrackers();

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

	UFUNCTION()
	virtual void OnRep_CurrentStage(UArcActivityStage* PreviousStage);

protected:
	template<typename T>
	void RaiseEvent(FGameplayTag Tag, const T& EventStruct)
	{
		if (UArcActivityWorldSubsystem* Subsys = GetWorld()->GetSubsystem<UArcActivityWorldSubsystem>())
		{
			Subsys->BroadcastMessage(Tag, EventStruct);
		}
	}

private:
	//BEGIN ACCESSED BY ARC ACTIVITY GI SUBSYSTEM
	bool InitActivityGraph(UArcActivity* Graph, const FGameplayTagContainer& Tags);

	void ProgressStage_Internal(EArcActivitySuccessState Transition);

	void EnterStage_Internal(UArcActivityStage* Stage);
	void ExitStage_Internal(UArcActivityStage* Stage);

	void TrackerUpdated_Internal(UArcActivityTask_ObjectiveTracker* Tracker);

	FArcActivityDelegateEnded OnActivityEnded;

	TWeakObjectPtr<UWorld> World;

	UPROPERTY(Replicated)
	TObjectPtr<UArcActivity> ActivityGraph;

	UPROPERTY(BlueprintReadOnly, Category = "Activity", meta = (AllowPrivateAccess), ReplicatedUsing = OnRep_CurrentStage)
	TObjectPtr<UArcActivityStage> CurrentStage;

	
	UPROPERTY(Replicated)
	FGameplayTagContainer ActivityTags;

	//TODO:FastArraySerialize this
	UPROPERTY(BlueprintReadOnly, Category="Activity", meta=(AllowPrivateAccess), Replicated)
	TArray<TObjectPtr<UArcActivityPlayerComponent>> PlayersInActivty;

	//TODO:FastArraySerialize this
	UPROPERTY(Replicated)
	TArray<TObjectPtr<UArcActivityTask_StageService>> CurrentGlobalStageServices;

	//TODO:FastArraySerialize this
	UPROPERTY(Replicated)
	TArray< TObjectPtr<UArcActivityTask_StageService>> CurrentStageServices;

	//TODO:FastArraySerialize this
	UPROPERTY(Replicated)
	TArray<TObjectPtr< UArcActivityTask_ObjectiveTracker>> CurrentObjectiveTrackers;


	UPROPERTY(Replicated)
	EArcActivitySuccessState ActivityState;
	
};
