// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcActivityReplicationProxy.h"
#include "ArcActivityInstance.h"

#include "Net/UnrealNetwork.h"

#include "Engine/ActorChannel.h" 
#include "Engine/NetDriver.h"
#include "Engine/NetConnection.h"

// Sets default values
AArcActivityReplicationProxy::AArcActivityReplicationProxy()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

bool AArcActivityReplicationProxy::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (const FArcActivityReplicationEntry& Instance : ReplicatedActivities)
	{
		if (IsValid(Instance.Activity))
		{
			bWroteSomething |= Channel->ReplicateSubobject(Instance.Activity, *Bunch, *RepFlags);
			bWroteSomething |= Instance.Activity->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}

	return bWroteSomething;
}

void AArcActivityReplicationProxy::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArcActivityReplicationProxy, ReplicatedActivities);
}

void AArcActivityReplicationProxy::AddReplicatedActivityInstance(UArcActivityInstance* Instance)
{
	check(IsValid(Instance));
	check(Instance->GetOuter() == this);

	FArcActivityReplicationEntry Entry(Instance);
	ReplicatedActivities.Items.Add(Entry);
	ReplicatedActivities.MarkItemDirty(Entry);

	if (IsUsingRegisteredSubObjectList())
	{
		AddReplicatedSubObject(Instance);
	}
}

void AArcActivityReplicationProxy::RemoveReplicatedActivityInstance(UArcActivityInstance* Instance)
{
	bool bRemovedElement = false;
	for (int i = 0; i < ReplicatedActivities.Num(); i++)
	{
		if (ReplicatedActivities[i].Activity == Instance)
		{
			ReplicatedActivities.Items.RemoveAtSwap(i);
			i--;
			bRemovedElement = true;
		}
	}

	if (bRemovedElement)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(Instance);
		}
		ReplicatedActivities.MarkArrayDirty();
	}
}

void FArcActivityReplicationEntry::PreReplicatedRemove(const FArcActivityReplicationArray& InArraySerializer)
{
}

void FArcActivityReplicationEntry::PostReplicatedAdd(const FArcActivityReplicationArray& InArraySerializer)
{
}

void FArcActivityReplicationEntry::PostReplicatedChange(const FArcActivityReplicationArray& InArraySerializer)
{
}

FString FArcActivityReplicationEntry::GetDebugString()
{
	return FString();
}
