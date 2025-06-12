// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#include "ArcActivityTypes.h"
#include "ArcActivityInstance.h"
#include "ArcActivityPlayerComponent.h"

UE_DEFINE_GAMEPLAY_TAG(FArcActivityStateChangedEventTag, TEXT("Activity.Event.StateChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityStageChangedEventTag, TEXT("Activity.Event.StageChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityPlayerChangedEventTag, TEXT("Activity.Event.PlayerChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityTagStackChangedEventTag, TEXT("Activity.Event.TagStackChanged"));


bool FArcActivityTaggedData::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Tag.NetSerialize(Ar, Map, bOutSuccess);
	Ar << Value;
	bOutSuccess = true;
	return true;
}

FString FArcActivityTaggedData::GetDebugString() const
{
	return TEXT("TODO");
}

void FArcActivityTaggedDataContainer::RebuildTagToCountMap()
{
	TagToDataMap.Empty(TaggedData.Num());
	for (const auto& [Tag, Data] : TagToDataMap)
	{
		TagToDataMap.Add(Tag, Data);
	}
}

void FArcActivityTaggedDataContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = TaggedData[Index].Tag;
		TagToDataMap.Remove(Tag);
		OnTaggedDataChanged.Broadcast(Tag, TaggedData[Index].Value, true);
	}
}

void FArcActivityTaggedDataContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FArcActivityTaggedData& Data = TaggedData[Index];
		TagToDataMap.Add(Data.Tag, Data.Value);
		OnTaggedDataChanged.Broadcast(Data.Tag, FTaggedDataVariant{}, false);
	}
}

void FArcActivityTaggedDataContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FArcActivityTaggedData& Data = TaggedData[Index];
		FTaggedDataVariant OldData = TagToDataMap.FindRef(Data.Tag);
		TagToDataMap[Data.Tag] = Data.Value;
		OnTaggedDataChanged.Broadcast(Data.Tag, OldData, false);

	}
}


void FArcActivityPlayerEntry::PreReplicatedRemove(const FArcActivityPlayerArray& InArraySerializer)
{
	if (IsValid(InArraySerializer.Owner))
	{
		InArraySerializer.Owner->RemovePlayerFromActivity(Player);
	}
	PreviousPlayer = Player;
}

void FArcActivityPlayerEntry::PostReplicatedAdd(const FArcActivityPlayerArray& InArraySerializer)
{
	if (IsValid(InArraySerializer.Owner))
	{
		InArraySerializer.Owner->AddPlayerToActivity(Player);
	}
	PreviousPlayer = Player;
}

void FArcActivityPlayerEntry::PostReplicatedChange(const FArcActivityPlayerArray& InArraySerializer)
{
	if(PreviousPlayer.IsValid() && IsValid(InArraySerializer.Owner))
	{
		InArraySerializer.Owner->RemovePlayerFromActivity(PreviousPlayer.Get());
	}
	if (IsValid(Player) && IsValid(InArraySerializer.Owner))
	{
		InArraySerializer.Owner->AddPlayerToActivity(Player);
	}
	PreviousPlayer = Player;
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