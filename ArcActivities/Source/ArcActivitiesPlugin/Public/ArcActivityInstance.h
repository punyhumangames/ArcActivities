// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.h"
#include "ArcActivityWorldSubsystem.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/World.h"
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
class ARCACTIVITIESPLUGIN_API UArcActivityInstance : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	friend class UArcActivityWorldSubsystem;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UArcActivityInstance();

	virtual class UWorld* GetWorld() const override;

	bool HasAuthority() const;

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool IsNameStableForNetworking() const override { return false; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsActive() const;
	void EndActivity(bool bWasCancelled = false);

	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	TArray<UArcActivityTask_ObjectiveTracker*> GetCurrentObjectiveTrackers();

	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	void AddPlayerToActivity(UArcActivityPlayerComponent* Player);
	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
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

	UFUNCTION(BlueprintPure, Category = "Arc|Activity")
	EArcActivitySuccessState GetActivityState() const { return ActivityState; }

	UFUNCTION()
	virtual void OnRep_CurrentStage(UArcActivityStage* PreviousStage);

	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	TArray<UArcActivityPlayerComponent*> GetPlayersInActivity() const;


	template<typename T>
	void SetTaggedData(FGameplayTag Tag, T Value) 
	{
		TaggedData.SetTaggedData(Tag, Value);
	}

	void ClearTaggedData(FGameplayTag Tag) 
	{
		TaggedData.ClearTaggedData(Tag);
	}

	template<typename T>
	TOptional<T> GetTaggedData(FGameplayTag Tag) const
	{
		return TaggedData.GetTaggedData<T>(Tag);
	}

	bool HasTaggedData(FGameplayTag Tag) const
	{
		return TaggedData.HasTaggedData(Tag);
	}


	

protected:
	void OnTagCountChanged(FGameplayTag Tag, int32 CurrentValue, int32 PreviousValue) const;

	template<typename T>
	void RaiseEvent(FGameplayTag Tag, const T& EventStruct) const;

private:
	//BEGIN ACCESSED BY ARC ACTIVITY GI SUBSYSTEM
	bool InitActivityGraph(UArcActivity* Graph, const FGameplayTagContainer& Tags);

	void ProgressStage_Internal(EArcActivitySuccessState Transition);

	void EnterStage_Internal(UArcActivityStage* Stage);
	void ExitStage_Internal(UArcActivityStage* Stage);

	void TrackerUpdated_Internal(UArcActivityTask_ObjectiveTracker* Tracker);

	float GetDelayTimeForStageTransition() const;

	FArcActivityDelegateEnded OnActivityEnded;

	TWeakObjectPtr<UWorld> World;



	UPROPERTY(Replicated)
		EArcActivitySuccessState ActivityState;

	UPROPERTY(Replicated)
		EArcActivitySuccessState PreviousStageCompletion;

	UPROPERTY(Replicated)
	TObjectPtr<UArcActivity> ActivityGraph;

	UPROPERTY(BlueprintReadOnly, Category = "Activity", meta = (AllowPrivateAccess), ReplicatedUsing = OnRep_CurrentStage)
	TObjectPtr<UArcActivityStage> CurrentStage;

	UPROPERTY(Replicated)
	FArcActivityTaggedDataContainer TaggedData;
	
	UPROPERTY(Replicated)
	FGameplayTagContainer ActivityTags;

	UPROPERTY(Replicated)
	FArcActivityPlayerArray PlayersInActivty;

	UPROPERTY()
	TArray<UArcActivityTask_StageService*> CurrentGlobalStageServices;

	UPROPERTY()
	TArray<UArcActivityTask_StageService*> CurrentStageServices;
	
	UPROPERTY()
	TArray<UArcActivityTask_ObjectiveTracker*> CurrentObjectiveTrackers;

	FTimerHandle StageDelayTimer;
};

template<typename T>
inline void UArcActivityInstance::RaiseEvent(FGameplayTag Tag, const T& EventStruct) const
{
	if (IsValid(GetWorld()))
	{
		if (UArcActivityWorldSubsystem* Subsys = GetWorld()->GetSubsystem<UArcActivityWorldSubsystem>())
		{
			Subsys->BroadcastMessage(Tag, EventStruct);
		}
	}
}