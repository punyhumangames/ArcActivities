// Copyright 2023 Puny Human.  All Rights Reserved


#include "ArcActivityPlayerComponent.h"
#include "ArcActivityInstance.h"


UArcActivityPlayerComponent::UArcActivityPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UArcActivityPlayerComponent::JoinActivity(UArcActivityInstance* ActivityInstance)
{
	if (IsValid(ActivityInstance))
	{
		ActivityInstance->AddPlayerToActivity(this);
	}
}

void UArcActivityPlayerComponent::LeaveActivity(UArcActivityInstance* ActivityInstance)
{
	if (IsValid(ActivityInstance))
	{
		ActivityInstance->RemovePlayerFromActivity(this);
	}
}

void UArcActivityPlayerComponent::OnPlayerJoinedActivity_Internal(UArcActivityInstance* ActivityInstance)
{
	if (IsValid(ActivityInstance))
	{
		Activities.AddUnique(ActivityInstance);
		//TODO: Broadcast an event
	}
}

void UArcActivityPlayerComponent::OnPlayerLeftActivity_Internal(UArcActivityInstance* ActivityInstance, bool ActivityEnding)
{
	if (IsValid(ActivityInstance))
	{
		Activities.RemoveSwap(ActivityInstance);
		//TODO: Broadcast an event
	}
}

