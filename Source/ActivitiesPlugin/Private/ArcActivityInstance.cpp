// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcActivityInstance.h"
#include "DataModel/Activity.h"
#include "DataModel/ActivityTask_StageService.h"
#include "DataModel/ObjectiveTracker.h"
#include "DataModel/ActivityObjective.h"
#include "ArcActivityPlayerComponent.h"

bool UArcActivityInstance::IsActive() const
{
	return CurrentStage != nullptr;
}

void UArcActivityInstance::EndActivity(bool bWasCancelled)
{
	//TODO: Cancel the current stage, givbing the tasks a chance to handle it


	//End the tasks and destroy them
	ForEachStageService_Mutable([bWasCancelled](UActivityTask_StageService* Service)
		{
			Service->EndPlay(bWasCancelled);
		});	
	ForEachObjectiveTracker_Mutable([bWasCancelled](UActivityTask_ObjectiveTracker* Tracker)
		{
			Tracker->EndPlay(bWasCancelled);
		});

	CurrentGlobalStageServices.Empty();
	CurrentStageServices.Empty();
	CurrentObjectiveTrackers.Empty();

	//Set this instance 
	CurrentStage = nullptr;
	OnActivityEnded.Broadcast(this, bWasCancelled);
}

void UArcActivityInstance::AddPlayerToActivity(UArcActivityPlayerComponent* Player)
{
	if (IsValid(Player))
	{
		PlayersInActivty.AddUnique(Player);
	}
}

void UArcActivityInstance::RemovePlayerFromActivity(UArcActivityPlayerComponent* Player)
{
	PlayersInActivty.Remove(Player);
}

bool UArcActivityInstance::TryProgressStage()
{
	if (!IsValid(CurrentStage))
	{
		return false;
	}

	

	return false; //TODO: Return true if success
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

void UArcActivityInstance::InitActivityGraph(UActivity* Graph, const FGameplayTagContainer& Tags)
{
	if (ensure(!IsValid(Graph)))
	{
		return;
	}

	ActivityGraph = Graph;
	CurrentStage = ActivityGraph->InitialStage;


	//Lets check if we can progress (the Initial stage is almost always "progress immediately"
	TryProgressStage();
}

void UArcActivityInstance::ProgressStage_Internal(EArcActivityTransitionMode Transition)
{
	check(!IsValid(CurrentStage));


}

void UArcActivityInstance::EnterStage_Internal(UActivityStage* Stage)
{
	CurrentStage = Stage;
	for (auto* Service : Stage->StageServices)
	{
		CurrentStageServices.AddUnique(DuplicateObject(Service, this));
	}
	for (auto Objective : Stage->Objectives)
	{
		if (IsValidChecked(Objective))
		{			
			for (auto Tracker : Objective->ObjectiveTrackers)
			{
				if (IsValidChecked(Tracker))
				{
					UActivityTask_ObjectiveTracker* NewTracker = DuplicateObject(Tracker, this);
					NewTracker->ObjectiveRef = Objective;
					CurrentObjectiveTrackers.AddUnique(NewTracker);
				}
			}
		}
	}
}

void UArcActivityInstance::ExitStage_Internal(UActivityStage* Stage)
{
}
