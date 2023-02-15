// Copyright 2023 Puny Human.  All Rights Reserved

#include "ArcActivityTypes.h"

UE_DEFINE_GAMEPLAY_TAG(FArcActivityStateChangedEventTag, TEXT("Activity.Event.StateChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityStageChangedEventTag, TEXT("Activity.Event.StageChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityPlayerChangedEventTag, TEXT("Activity.Event.PlayerChanged"));

void FArcActivityTaskEntry::PreReplicatedRemove(const FArcActivityTaskArray& InArraySerializer)
{
}

void FArcActivityTaskEntry::PostReplicatedAdd(const FArcActivityTaskArray& InArraySerializer)
{
}

void FArcActivityTaskEntry::PostReplicatedChange(const FArcActivityTaskArray& InArraySerializer)
{
}

FString FArcActivityTaskEntry::GetDebugString()
{
	return FString();
}

void FArcActivityTaskArray::Reset()
{
	Items.Reset();
	MarkArrayDirty();
}

void FArcActivityTaskArray::Add(UArcActivityTask_Base* Service)
{
	FArcActivityTaskEntry& entry = Items.Add_GetRef(FArcActivityTaskEntry(Service));
	MarkItemDirty(entry);
}


void FArcActivityPlayerEntry::PreReplicatedRemove(const FArcActivityPlayerArray& InArraySerializer)
{
}

void FArcActivityPlayerEntry::PostReplicatedAdd(const FArcActivityPlayerArray& InArraySerializer)
{
}

void FArcActivityPlayerEntry::PostReplicatedChange(const FArcActivityPlayerArray& InArraySerializer)
{
}

FString FArcActivityPlayerEntry::GetDebugString()
{
	return FString();
}

void FArcActivityPlayerArray::Reset()
{
	Items.Reset();
	MarkArrayDirty();
}

void FArcActivityPlayerArray::Add(UArcActivityPlayerComponent* Player)
{
	FArcActivityPlayerEntry& entry = Items.Add_GetRef(FArcActivityPlayerEntry(Player));
	MarkItemDirty(entry);
}

void FArcActivityPlayerArray::Remove(UArcActivityPlayerComponent* Player)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Player == Player)
		{
			Items.RemoveAt(i);
			i--;
		}
	}
}
