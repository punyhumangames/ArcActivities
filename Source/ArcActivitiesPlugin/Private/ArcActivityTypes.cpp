// Copyright 2023 Puny Human.  All Rights Reserved

#include "ArcActivityTypes.h"
#include "ArcActivityInstance.h"
#include "ArcActivityPlayerComponent.h"

UE_DEFINE_GAMEPLAY_TAG(FArcActivityStateChangedEventTag, TEXT("Activity.Event.StateChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityStageChangedEventTag, TEXT("Activity.Event.StageChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityPlayerChangedEventTag, TEXT("Activity.Event.PlayerChanged"));
UE_DEFINE_GAMEPLAY_TAG(FArcActivityTagStackChangedEventTag, TEXT("Activity.Event.TagStackChanged"));



FString FArcActivityTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

void FArcActivityTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FArcActivityTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				MarkItemDirty(Stack);
				return;
			}
		}

		FArcActivityTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
		OnTagCountChanged.Broadcast(Tag, StackCount, 0);
	}
}

void FArcActivityTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FArcActivityTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{

					OnTagCountChanged.Broadcast(Tag, 0, Stack.StackCount);
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
					MarkArrayDirty();
				}
				else
				{
					const int32 OldCount = Stack.StackCount;
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
					OnTagCountChanged.Broadcast(Tag, NewCount,OldCount);
				}
				return;
			}
		}
	}
}

void FArcActivityTagStackContainer::SetStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FArcActivityTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 OldCount = Stack.StackCount;
				Stack.StackCount = StackCount;
				TagToCountMap[Tag] = StackCount;
				MarkItemDirty(Stack);
				OnTagCountChanged.Broadcast(Tag, StackCount, OldCount);
				return;
			}
		}

		FArcActivityTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
		OnTagCountChanged.Broadcast(Tag, StackCount, 0);
	}
}

void FArcActivityTagStackContainer::RebuildTagToCountMap()
{
	TagToCountMap.Empty(Stacks.Num());
	for (const auto& Stack : Stacks)
	{
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
	}
}

void FArcActivityTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[Index].Tag;
		TagToCountMap.Remove(Tag);
		OnTagCountChanged.Broadcast(Tag, 0, Stacks[Index].StackCount);
	}
}

void FArcActivityTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FArcActivityTagStack& Stack = Stacks[Index];
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
		OnTagCountChanged.Broadcast(Stack.Tag, Stacks[Index].StackCount, 0);
	}
}

void FArcActivityTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		
		const FArcActivityTagStack& Stack = Stacks[Index];
		const int32 OldCount = TagToCountMap.FindRef(Stack.Tag);
		TagToCountMap[Stack.Tag] = Stack.StackCount;
		OnTagCountChanged.Broadcast(Stack.Tag, Stack.StackCount, OldCount);
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
	if(IsValid(PreviousPlayer) && IsValid(InArraySerializer.Owner))
	{
		InArraySerializer.Owner->RemovePlayerFromActivity(PreviousPlayer);
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