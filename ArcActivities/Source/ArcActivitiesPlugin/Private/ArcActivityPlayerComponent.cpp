// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info



#include "ArcActivityPlayerComponent.h"
#include "ArcActivityInstance.h"


UArcActivityPlayerComponent::UArcActivityPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
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

