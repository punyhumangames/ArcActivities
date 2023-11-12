// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.h"
#include "ArcActivityWorldSubsystem.h"
#include "GameplayTagAssetInterface.h"
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

	UFUNCTION(BlueprintCallable)
	TArray<UArcActivityPlayerComponent*> GetPlayersInActivity() const;

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Arc|Activity")
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Arc|Activity")
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Sets a stat tag stack
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Arc|Activity")
	void SetStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	void ClearStatTag(FGameplayTag Tag);


protected:
	void OnTagCountChanged(FGameplayTag Tag, int32 CurrentValue, int32 PreviousValue) const;

	template<typename T>
	void RaiseEvent(FGameplayTag Tag, const T& EventStruct) const
	{
		if (IsValid(GetWorld()))
		{
			if (UArcActivityWorldSubsystem* Subsys = GetWorld()->GetSubsystem<UArcActivityWorldSubsystem>())
			{
				Subsys->BroadcastMessage(Tag, EventStruct);
			}
		}		
	}

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
	FArcActivityTagStackContainer TagStacks;
	
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
