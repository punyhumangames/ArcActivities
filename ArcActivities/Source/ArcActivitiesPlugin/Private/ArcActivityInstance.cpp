// Copyright 2023 Puny Human.  All Rights Reserved


#include "ArcActivityInstance.h"
#include "Engine/World.h"
#include "DataModel/ArcActivity.h"
#include "DataModel/ArcActivityTask_StageService.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "DataModel/ArcActivityObjective.h"
#include "DataModel/ArcActivityStage.h"
#include "ArcActivityPlayerComponent.h"
#include "ArcActivityPlayerComponent.h"
#include "ArcActivityReplicationProxy.h"

#include "Net/UnrealNetwork.h"

#include "Engine/ActorChannel.h" 
#include "Engine/NetDriver.h"
#include "Engine/NetConnection.h"

#include "TimerManager.h"

void UArcActivityInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (IsValid(ActivityGraph))
	{
		ActivityGraph->GetOwnedGameplayTags(TagContainer);
	}
	
	TagContainer.AppendTags(ActivityTags);	
}

UArcActivityInstance::UArcActivityInstance()
	: Super()
	, PreviousStageCompletion(EArcActivitySuccessState::None)
	, PlayersInActivty(this)
{
	TagStacks.OnTagCountChanged.AddUObject(this, &ThisClass::OnTagCountChanged);
}

UWorld* UArcActivityInstance::GetWorld() const
{
	//If we're constructed from a RepProxy (in multiplayer environments), our world may not be directly set during construction
	//So lets make sure we get our world from it
	if (AArcActivityReplicationProxy* RepProxy = Cast<AArcActivityReplicationProxy>(GetOuter()))
	{
		return RepProxy->GetWorld();
	}
	return World.Get();
}

bool UArcActivityInstance::HasAuthority() const
{
	if (GetWorld()->GetNetMode() == NM_Client)
	{
		return false;
	}
	return true;
}

void UArcActivityInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UArcActivityInstance, ActivityGraph, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UArcActivityInstance, ActivityTags, COND_InitialOnly);
	DOREPLIFETIME(UArcActivityInstance, CurrentStage);
	DOREPLIFETIME(UArcActivityInstance, PlayersInActivty);
	DOREPLIFETIME(UArcActivityInstance, ActivityState);
	DOREPLIFETIME(UArcActivityInstance, PreviousStageCompletion);
	DOREPLIFETIME(UArcActivityInstance, TagStacks);
}

bool UArcActivityInstance::IsActive() const
{
	return CurrentStage != nullptr;
}

void UArcActivityInstance::EndActivity(bool bWasCancelled)
{	
	if (bWasCancelled)
	{
		ActivityState = EArcActivitySuccessState::Cancelled;
		ForEachStageService_Mutable([this](UArcActivityTask_StageService* Service)
		{
			Service->StageEnded(ActivityState);
		});
	}
	if (ActivityState == EArcActivitySuccessState::InProgress)
	{
		//Assert here, this is invalid
	}

	RaiseEvent(FArcActivityStateChangedEventTag, FArcActivityActivityStateChanged(this, ActivityState, EArcActivitySuccessState::InProgress));

	for (const auto& PlayerComp : PlayersInActivty)
	{
		if (IsValid(PlayerComp.Player))
		{
			PlayerComp.Player->OnPlayerLeftActivity_Internal(this, true);
		}
	}
	
	//End the tasks and destroy them
	ForEachStageService_Mutable([bWasCancelled](UArcActivityTask_StageService* Service)
		{
			Service->EndPlay(bWasCancelled);
		});	
	ForEachObjectiveTracker_Mutable([bWasCancelled](UArcActivityTask_ObjectiveTracker* Tracker)
		{
			Tracker->EndPlay(bWasCancelled);
		});

	if (HasAuthority())
	{
		PlayersInActivty.Reset();
		CurrentGlobalStageServices.Reset();
		CurrentStageServices.Reset();
		CurrentObjectiveTrackers.Reset();
	}

	//Set this instance 
	CurrentStage = nullptr;
	OnActivityEnded.Broadcast(this, bWasCancelled);
}

TArray<UArcActivityTask_ObjectiveTracker*> UArcActivityInstance::GetCurrentObjectiveTrackers()
{
	TArray<UArcActivityTask_ObjectiveTracker*> Elements;
	ForEachObjectiveTracker_Mutable([&Elements](UArcActivityTask_ObjectiveTracker* Entry)
		{
			Elements.Add(Entry);
		});
	return MoveTemp(Elements);
}

void UArcActivityInstance::AddPlayerToActivity(UArcActivityPlayerComponent* Player)
{
	if (IsValid(Player))
	{
		if (HasAuthority())
		{
			PlayersInActivty.Add(Player);
		}

		Player->OnPlayerJoinedActivity_Internal(this);
		RaiseEvent(FArcActivityPlayerChangedEventTag, FArcActivityPlayerEventPayload(this, Player, EArcActivityPlayerEventType::PlayerJoined));
	}
}

void UArcActivityInstance::RemovePlayerFromActivity(UArcActivityPlayerComponent* Player)
{
	if (IsValid(Player))
	{
		RaiseEvent(FArcActivityPlayerChangedEventTag, FArcActivityPlayerEventPayload(this, Player, EArcActivityPlayerEventType::PlayerLeft));
	
		if (HasAuthority())
		{
			PlayersInActivty.Remove(Player);
		}
		Player->OnPlayerLeftActivity_Internal(this);
	}
}

bool UArcActivityInstance::TryProgressStage()
{
	if (!HasAuthority())
	{
		return false;
	}

	if (!IsValid(CurrentStage))
	{
		return false;
	}

	EArcActivityCompletionMode StageCompletionMode = CurrentStage->CompletionMode;

	
	TMap<UArcActivityObjective*, EArcActivityObjectiveTrackerState> ObjectiveStates;

	
	EArcActivityObjectiveTrackerState StageState = EArcActivityObjectiveTrackerState::Tracking;

	const bool bStageAnyFail = StageCompletionMode == EArcActivityCompletionMode::AllSuccess;
	const bool bStageAnySuccess = StageCompletionMode == EArcActivityCompletionMode::AnySuccess;


	//Check the status of the trackers
	for (auto Tracker : CurrentObjectiveTrackers)
	{		
		if (!IsValid(Tracker))
		{
			continue;
		}

		EArcActivityObjectiveTrackerState TrackerState = Tracker->GetTrackerState();
		const EArcActivityCompletionMode ObjectiveCompletionMode = Tracker->Objective->CompletionMode;
	
		const bool bObjectiveAnyFail = ObjectiveCompletionMode == EArcActivityCompletionMode::AllSuccess;
		const bool bObjectiveAnySuccess = ObjectiveCompletionMode == EArcActivityCompletionMode::AnySuccess;
		
		//If the stage fails on any fail and any objective is any fail, the first failure fails this entire stage
		if (bStageAnyFail && bObjectiveAnyFail && TrackerState == EArcActivityObjectiveTrackerState::CompletedFail)
		{
			StageState = EArcActivityObjectiveTrackerState::CompletedFail;
			break;
		}		
		//If the stage succeeds on any success and one of the objectives succeeds on any success, the first success succeeds this whole stage
		if (bStageAnySuccess && bObjectiveAnySuccess && TrackerState == EArcActivityObjectiveTrackerState::CompletedSuccess)
		{
			StageState = EArcActivityObjectiveTrackerState::CompletedSuccess;
			break;
		}

		EArcActivityObjectiveTrackerState& ObjState = ObjectiveStates.FindOrAdd(Tracker->Objective, ObjectiveCompletionMode == EArcActivityCompletionMode::AllSuccess ? EArcActivityObjectiveTrackerState::CompletedSuccess : EArcActivityObjectiveTrackerState::CompletedFail);


		bool bObjectiveDone = bObjectiveAnySuccess && ObjState == EArcActivityObjectiveTrackerState::CompletedSuccess
			|| bObjectiveAnyFail && ObjState == EArcActivityObjectiveTrackerState::CompletedFail;
		if (bObjectiveDone)
		{
			continue;
		}

		if (bObjectiveAnySuccess && TrackerState != EArcActivityObjectiveTrackerState::CompletedFail)
		{
			if (TrackerState == EArcActivityObjectiveTrackerState::CompletedSuccess)
			{
				ObjState = EArcActivityObjectiveTrackerState::CompletedSuccess;
				continue;
			}
			
			ObjState = EArcActivityObjectiveTrackerState::Tracking;
			
		}
		else if (bObjectiveAnyFail && TrackerState != EArcActivityObjectiveTrackerState::CompletedSuccess)
		{
			if (TrackerState == EArcActivityObjectiveTrackerState::CompletedFail)
			{
				ObjState = EArcActivityObjectiveTrackerState::CompletedFail;
				continue;
			}
			ObjState = EArcActivityObjectiveTrackerState::Tracking;
		}
	}

	//If we're still tracking after collecting completions, check to see if any of the AllSuccess succeeded
	if (StageState == EArcActivityObjectiveTrackerState::Tracking)
	{
		int32 NumSuccess = 0;
		int32 NumComplete = 0;
		for (const auto& KV : ObjectiveStates)
		{
			if (bStageAnyFail && KV.Value == EArcActivityObjectiveTrackerState::CompletedFail)
			{
				StageState = EArcActivityObjectiveTrackerState::CompletedFail;
				break;
			}
			if (KV.Value != EArcActivityObjectiveTrackerState::Tracking)
			{
				NumComplete++;
			}
			if (KV.Value == EArcActivityObjectiveTrackerState::CompletedSuccess)
			{
				NumSuccess++;
			}
		}

		if (NumComplete == CurrentStage->Objectives.Num())//All objectives have completed.  We're not waiting on any			 
		{
			if (NumSuccess != NumComplete)//And one of them has failed
			{
				StageState = EArcActivityObjectiveTrackerState::CompletedFail;
			}
			else
			{
				StageState = EArcActivityObjectiveTrackerState::CompletedSuccess;
			}
			
		}
	}

	if (StageState == EArcActivityObjectiveTrackerState::Tracking)
	{
		return false;
	}

	EArcActivitySuccessState Transition = StageState == EArcActivityObjectiveTrackerState::CompletedSuccess ? EArcActivitySuccessState::Success : EArcActivitySuccessState::Failure;
	float TotalTime = GetDelayTimeForStageTransition();
	if (TotalTime > 0)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::ProgressStage_Internal, Transition);
		GetWorld()->GetTimerManager().SetTimer(StageDelayTimer, TimerDelegate, TotalTime, false);
	}
	else
	{
		ProgressStage_Internal(Transition);
	}
	return true;	
}

void UArcActivityInstance::ForEachGlobalStageService(ConstForEachStageServiceFunc Func) const
{
	for (auto StageService : CurrentGlobalStageServices)
	{
		if (IsValid(StageService))
		{
			Func(StageService);
		}
	}
}

void UArcActivityInstance::ForEachCurrentStageService(ConstForEachStageServiceFunc Func) const
{
	for (auto StageService : CurrentStageServices)
	{
		if (IsValid(StageService))
		{
			Func(StageService);
		}
	}
}

void UArcActivityInstance::ForEachGlobalStageService_Mutable(ForEachStageServiceFunc Func) const
{
	for (auto StageService : CurrentGlobalStageServices)
	{
		if (IsValid(StageService))
		{
			Func(StageService);
		}
	}
}

void UArcActivityInstance::ForEachCurrentStageService_Mutable(ForEachStageServiceFunc Func) const
{
	for (auto StageService : CurrentStageServices)
	{
		if (IsValid(StageService))
		{
			Func(StageService);
		}
	}
}


void UArcActivityInstance::ForEachObjectiveTracker(ConstForEachObjectiveTrackerFunc Func) const
{
	for (auto Tracker : CurrentObjectiveTrackers)
	{
		if (IsValid(Tracker))
		{
			Func(Tracker);
		}
	}
}

void UArcActivityInstance::ForEachObjectiveTracker_Mutable(ForEachObjectiveTrackerFunc Func) const
{
	for (auto Tracker : CurrentObjectiveTrackers)
	{
		if (IsValid(Tracker))
		{
			Func(Tracker);
		}
	}
}

void UArcActivityInstance::OnRep_CurrentStage(UArcActivityStage* PreviousStage)
{
	if (!IsValid(PreviousStage))
	{
		RaiseEvent(FArcActivityStateChangedEventTag, FArcActivityActivityStateChanged(this, EArcActivitySuccessState::InProgress, EArcActivitySuccessState::None));
	}			
	RaiseEvent(FArcActivityStageChangedEventTag, FArcActivityStageChangedEventPayload(this, CurrentStage, PreviousStage, PreviousStageCompletion));
	
}

TArray<UArcActivityPlayerComponent*> UArcActivityInstance::GetPlayersInActivity() const
{
	TArray<UArcActivityPlayerComponent*> Components;
	Components.Reserve(PlayersInActivty.Items.Num());

	for (const auto& Player : PlayersInActivty)
	{
		Components.AddUnique(Player.Player);
	}
	return Components;
}

void UArcActivityInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	TagStacks.AddStack(Tag, StackCount);
}

void UArcActivityInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	TagStacks.RemoveStack(Tag, StackCount);
}

void UArcActivityInstance::SetStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	TagStacks.SetStack(Tag, StackCount);
}

int32 UArcActivityInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return TagStacks.GetStackCount(Tag);
}

bool UArcActivityInstance::HasStatTag(FGameplayTag Tag) const
{
	return TagStacks.ContainsTag(Tag);
}

void UArcActivityInstance::ClearStatTag(FGameplayTag Tag)
{
	TagStacks.ClearStack(Tag);
}

void UArcActivityInstance::OnTagCountChanged(FGameplayTag Tag, int32 CurrentValue, int32 PreviousValue) const
{
	RaiseEvent(FArcActivityTagStackChangedEventTag, FArcActivityTagStackChanged( const_cast<UArcActivityInstance*>(this), Tag, CurrentValue, PreviousValue ));
}

bool UArcActivityInstance::InitActivityGraph(UArcActivity* Graph, const FGameplayTagContainer& Tags)
{
	if (!ensure(IsValid(Graph)))
	{
		return false;
	}
	if (!ensure(IsValid(Graph->InitialStage)))
	{
		UE_LOG(LogActivitiesPlugin, Error, TEXT("Cannot Start Activity Instance for activity %s: It doesn't have an initial stage"), *GetNameSafe(Graph));
		return false;
	}
	
	ActivityTags.AppendTags(Tags);
	ActivityGraph = Graph;
	EnterStage_Internal(ActivityGraph->InitialStage);

	ForEachStageService_Mutable([](UArcActivityTask_StageService* StageService)
	{
		StageService->StageBegun(EArcActivitySuccessState::Success, nullptr);
	});

	RaiseEvent(FArcActivityStateChangedEventTag, FArcActivityActivityStateChanged(this, EArcActivitySuccessState::InProgress, EArcActivitySuccessState::None));
	RaiseEvent(FArcActivityStageChangedEventTag, FArcActivityStageChangedEventPayload(this, ActivityGraph->InitialStage, nullptr, EArcActivitySuccessState::None));


	//Lets check if we can progress (in case the initial stage is already done)
	TryProgressStage();
	return true;
}

void UArcActivityInstance::ProgressStage_Internal(EArcActivitySuccessState Transition)
{
	check(IsValid(CurrentStage));

	UArcActivityStage* PreviousStage = CurrentStage;

	UArcActivityStage* NextStage = nullptr;
	if (Transition == EArcActivitySuccessState::Success)
	{
		NextStage = CurrentStage->NextStage_Success;
	}
	else if (Transition == EArcActivitySuccessState::Failure)
	{
		NextStage = CurrentStage->NextStage_Failure;
	}

	if (IsValid(CurrentStage))
	{
		ForEachStageService_Mutable([Transition](UArcActivityTask_StageService* StageService)
			{
				StageService->StageEnded(Transition);
			});
		ExitStage_Internal(CurrentStage);
	}
	PreviousStageCompletion = Transition;
	CurrentStage = nullptr;
	
	if (IsValid(NextStage))
	{		
		EnterStage_Internal(NextStage);

		ForEachStageService_Mutable([Transition, PreviousStage](UArcActivityTask_StageService* StageService)
		{
			StageService->StageBegun(Transition, PreviousStage);
		});
	}

	RaiseEvent(FArcActivityStageChangedEventTag, FArcActivityStageChangedEventPayload(this, NextStage, PreviousStage, Transition));

	if(!IsValid(NextStage))
	{		
		ActivityState = Transition;
		//If we dont have a stage, we're done.
		EndActivity(false);
	}


}

void UArcActivityInstance::EnterStage_Internal(UArcActivityStage* Stage)
{
	CurrentStage = Stage;
	if (HasAuthority())
	{
		for (auto* Service : Stage->StageServices)
		{
			auto NewService = DuplicateObject(Service, this);
			NewService->OwningStage = Stage;
			CurrentStageServices.Add(NewService);
		}
		for (auto Objective : Stage->Objectives)
		{
			if (IsValidChecked(Objective))
			{
				for (auto Tracker : Objective->ObjectiveTrackers)
				{
					if (IsValidChecked(Tracker))
					{
						UArcActivityTask_ObjectiveTracker* NewTracker = DuplicateObject(Tracker, this);
						NewTracker->Objective = Objective;
						NewTracker->OnTrackerStateUpdated.AddUObject(this, &ThisClass::TrackerUpdated_Internal);
						CurrentObjectiveTrackers.Add(NewTracker);
					}
				}
			}
		}
	}
	
	ForEachCurrentStageService_Mutable([](UArcActivityTask_StageService* StageService)
	{
		StageService->BeginPlay();
	});

	ForEachObjectiveTracker_Mutable([](auto Tracker)
	{
		Tracker->BeginPlay();
	});

}

void UArcActivityInstance::ExitStage_Internal(UArcActivityStage* Stage)
{
	ForEachCurrentStageService_Mutable([](UArcActivityTask_StageService* StageService)
	{
		StageService->EndPlay(false);
	});

	ForEachObjectiveTracker_Mutable([this](UArcActivityTask_ObjectiveTracker* Tracker)
	{
		Tracker->EndPlay(false);
		Tracker->OnTrackerStateUpdated.RemoveAll(this);
	});

	if (HasAuthority())
	{
		CurrentStage = nullptr;
		CurrentStageServices.Reset();
		CurrentObjectiveTrackers.Reset();
	}
}

void UArcActivityInstance::TrackerUpdated_Internal(UArcActivityTask_ObjectiveTracker* Tracker)
{
	if (CurrentObjectiveTrackers.Contains(Tracker))
	{
		TryProgressStage();
	}
}

float UArcActivityInstance::GetDelayTimeForStageTransition() const
{
	float DelayTime = 0.0f;

	if (IsValid(ActivityGraph))
	{
		DelayTime += ActivityGraph->BaseStageProgressionTimer;
	}
	if (IsValid(CurrentStage))
	{
		DelayTime += CurrentStage->StageCompletionDelay;
	}

	return DelayTime;
}
