// Copyright 2023 Puny Human.  All Rights Reserved


#include "ArcActivityInstance.h"
#include "DataModel/ArcActivity.h"
#include "DataModel/ArcActivityTask_StageService.h"
#include "DataModel/ArcActivityTask_ObjectiveTracker.h"
#include "DataModel/ArcActivityObjective.h"
#include "DataModel/ArcActivityStage.h"
#include "ArcActivityPlayerComponent.h"
#include "ArcActivityPlayerComponent.h"

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

	for (UArcActivityPlayerComponent* PlayerComp : PlayersInActivty)
	{
		PlayerComp->OnPlayerLeftActivity_Internal(this, true);
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


	PlayersInActivty.Reset();
	CurrentGlobalStageServices.Reset();
	CurrentStageServices.Reset();
	CurrentObjectiveTrackers.Reset();

	//Set this instance 
	CurrentStage = nullptr;
	OnActivityEnded.Broadcast(this, bWasCancelled);
}

void UArcActivityInstance::AddPlayerToActivity(UArcActivityPlayerComponent* Player)
{
	if (IsValid(Player))
	{
		PlayersInActivty.AddUnique(Player);
		Player->OnPlayerJoinedActivity_Internal(this);

		RaiseEvent(FArcActivityPlayerChangedEventTag, FArcActivityPlayerEventPayload(this, Player, EArcActivityPlayerEventType::PlayerJoined));
	}
}

void UArcActivityInstance::RemovePlayerFromActivity(UArcActivityPlayerComponent* Player)
{
	if (IsValid(Player))
	{
		RaiseEvent(FArcActivityPlayerChangedEventTag, FArcActivityPlayerEventPayload(this, Player, EArcActivityPlayerEventType::PlayerLeft));
	
		PlayersInActivty.RemoveSwap(Player);	
		Player->OnPlayerLeftActivity_Internal(this);
	}
}

bool UArcActivityInstance::TryProgressStage()
{
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
	for (UArcActivityTask_ObjectiveTracker* Tracker : CurrentObjectiveTrackers)
	{
		EArcActivityObjectiveTrackerState TrackerState = Tracker->GetTrackerState();
		const EArcActivityCompletionMode ObjectiveCompletionMode = Tracker->ObjectiveRef->CompletionMode;
	
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

		EArcActivityObjectiveTrackerState& ObjState = ObjectiveStates.FindOrAdd(Tracker->ObjectiveRef, ObjectiveCompletionMode == EArcActivityCompletionMode::AllSuccess ? EArcActivityObjectiveTrackerState::CompletedSuccess : EArcActivityObjectiveTrackerState::CompletedFail);


		bool bObjectiveDone = bObjectiveAnySuccess && ObjState == EArcActivityObjectiveTrackerState::CompletedSuccess
			|| bObjectiveAnyFail && ObjState == EArcActivityObjectiveTrackerState::CompletedFail;
		if (bObjectiveDone)
		{
			continue;
		}

		if (bObjectiveAnySuccess && TrackerState != EArcActivityObjectiveTrackerState::CompletedFail)
		{
			if (ObjState == EArcActivityObjectiveTrackerState::CompletedSuccess)
			{
				ObjState = EArcActivityObjectiveTrackerState::CompletedSuccess;
				continue;
			}
			
			ObjState = EArcActivityObjectiveTrackerState::Tracking;
			
		}
		else if (bObjectiveAnyFail && TrackerState != EArcActivityObjectiveTrackerState::CompletedSuccess)
		{
			if (ObjState != EArcActivityObjectiveTrackerState::CompletedFail)
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


	ProgressStage_Internal(StageState == EArcActivityObjectiveTrackerState::CompletedSuccess ? EArcActivitySuccessState::Success : EArcActivitySuccessState::Failure);
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

	ActivityGraph = Graph;
	EnterStage_Internal(ActivityGraph->InitialStage);

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
	for (auto* Service : Stage->StageServices)
	{
		auto NewService = DuplicateObject(Service, this);
		NewService->OwningStage = Stage;
		CurrentStageServices.AddUnique(NewService);
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
					NewTracker->ObjectiveRef = Objective;
					NewTracker->OnTrackerStateUpdated.AddUObject(this, &ThisClass::TrackerUpdated_Internal);
					CurrentObjectiveTrackers.AddUnique(NewTracker);
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

	CurrentStage = nullptr;
	CurrentStageServices.Reset();
	CurrentObjectiveTrackers.Reset();
}

void UArcActivityInstance::TrackerUpdated_Internal(UArcActivityTask_ObjectiveTracker* Tracker)
{
	if (CurrentObjectiveTrackers.Contains(Tracker))
	{
		TryProgressStage();
	}
}
