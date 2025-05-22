// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Engine/NetSerialization.h"
#include "Misc/TVariant.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ArcActivityTypes.generated.h"


ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityStateChangedEventTag);
ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityStageChangedEventTag);
ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityPlayerChangedEventTag);
ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityTagStackChangedEventTag);

#define ARC_ACT_FASTARRAYSERIALIZER_TARRAY_ACCESSORS(PropertyName) \
		auto begin() { return PropertyName.begin(); }	\
		auto begin() const { return PropertyName.begin(); } \
		auto end() { return PropertyName.end(); } \
		auto end() const { return PropertyName.end(); } \
		auto& operator[](int32 index) { return PropertyName[index]; } \
		const auto& operator[](int32 index) const { return PropertyName[index]; } \
		int32 Num() const { return PropertyName.Num(); } 



class UArcActivityTask_Base;

class UArcActivityInstance;
class UArcActivityStage;
class UArcActivityPlayerComponent;

UENUM(BlueprintType)
enum class EArcActivityObjectiveTrackerState : uint8
{
	Tracking,
	CompletedSuccess,
	CompletedFail,
};

UENUM(BlueprintType)
enum class EArcActivitySuccessState : uint8
{
	Success,
	Failure,
	Cancelled,
	InProgress,
	None,
};


UENUM(BlueprintType)
enum class EArcActivityCompletionMode : uint8
{	
	//Will mark this as success if any child has succeeded. 
	AnySuccess,
	//Will mark as success if all are successful, fail if any failed
	AllSuccess,
};

// Match rule for message listeners
UENUM(BlueprintType)
enum class EArcActivityMessageMatch : uint8
{
	// An exact match will only receive messages with exactly the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B but not A.B.C)
	ExactMatch,

	// A partial match will receive any messages rooted in the same channel
	// (e.g., registering for "A.B" will match a broadcast of A.B as well as A.B.C)
	PartialMatch
};

/**
 * Struct used to specify advanced behavior when registering a listener for gameplay messages
 */
template<typename FMessageStructType>
struct FArcActivityListenerParams
{
	/** Whether Callback should be called for broadcasts of more derived channels or if it will only be called for exact matches. */
	EArcActivityMessageMatch MatchType = EArcActivityMessageMatch::ExactMatch;

	/** If bound this callback will trigger when a message is broadcast on the specified channel. */
	TFunction<void(FGameplayTag, const FMessageStructType&)> OnMessageReceivedCallback;

	/** Helper to bind weak member function to OnMessageReceivedCallback */
	template<typename TOwner = UObject>
	void SetMessageReceivedCallback(TOwner* Object, void(TOwner::* Function)(FGameplayTag, const FMessageStructType&))
	{
		TWeakObjectPtr<TOwner> WeakObject(Object);
		OnMessageReceivedCallback = [WeakObject, Function](FGameplayTag Channel, const FMessageStructType& Payload)
		{
			if (TOwner* StrongObject = WeakObject.Get())
			{
				(StrongObject->*Function)(Channel, Payload);
			}
		};
	}
};

USTRUCT(BlueprintType)
struct FArcActivityActivityStateChanged
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category="Activity")
	UArcActivityInstance* ActivityInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		EArcActivitySuccessState ActivityState = EArcActivitySuccessState::None;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	EArcActivitySuccessState PreviousActivityState = EArcActivitySuccessState::None;

	FArcActivityActivityStateChanged() { }

	FArcActivityActivityStateChanged(UArcActivityInstance* InActivityInstance,
		EArcActivitySuccessState InActivityState,
		EArcActivitySuccessState InPreviousActivityState)
		: ActivityInstance(InActivityInstance)
		, ActivityState(InActivityState)
		, PreviousActivityState(InPreviousActivityState)
	{

	}
};

USTRUCT(BlueprintType)
struct FArcActivityStageChangedEventPayload
{
	GENERATED_BODY()

		//The Activity that is raising this event
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityInstance* ActivityInstance = nullptr;

	//The stage the activity is currently in. May be null if the activity is completed
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		UArcActivityStage* CurrentStage = nullptr;

	//The stage the activity has left.  May be null if the activity is starting
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityStage* PreviousStage = nullptr;


	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		EArcActivitySuccessState PreviousStageState = EArcActivitySuccessState::None;

	FArcActivityStageChangedEventPayload() { } 

	FArcActivityStageChangedEventPayload(UArcActivityInstance* InActivityInstance,
		UArcActivityStage* InCurrentStage,
		UArcActivityStage* InPreviousStage,
		EArcActivitySuccessState InPreviousStageState)
		: ActivityInstance(InActivityInstance)
		, CurrentStage(InCurrentStage)
		, PreviousStage(InPreviousStage)
		, PreviousStageState(InPreviousStageState)
	{

	}
};


USTRUCT(BlueprintType)
struct FArcActivityTagStackChanged
{
	GENERATED_BODY()

	FArcActivityTagStackChanged() { }

	FArcActivityTagStackChanged(UArcActivityInstance* InInstance, FGameplayTag InTag, int32 InCurrentValue, int32 InPreviousValue)
		: Activity(InInstance)
		, Tag(InTag)
		, CurrentValue(InCurrentValue)
		, PreviousValue(InPreviousValue)
	{

	}

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		UArcActivityInstance* Activity;
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		FGameplayTag Tag;
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		int32 CurrentValue = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		int32 PreviousValue = 0;
};

UENUM(BlueprintType)
enum class EArcActivityPlayerEventType : uint8
{
	PlayerJoined,
	PlayerLeft
};

USTRUCT(BlueprintType)
struct FArcActivityPlayerEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityInstance* ActivityInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityPlayerComponent* Player = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
		EArcActivityPlayerEventType EventType = EArcActivityPlayerEventType::PlayerJoined;

	FArcActivityPlayerEventPayload() { }

	FArcActivityPlayerEventPayload(UArcActivityInstance* InActivityInstance,
		UArcActivityPlayerComponent* InPlayer,
		EArcActivityPlayerEventType InEventType)
		: ActivityInstance(InActivityInstance)
		, Player(InPlayer)
		, EventType(InEventType)
	{

	}
};

using FTaggedDataVariant = TVariant<int32, float, double, TWeakObjectPtr<AActor>, FVector, FGameplayTag>;	

USTRUCT(BlueprintType)
struct FArcActivityTaggedData : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FArcActivityTaggedData()
	{}

	template<typename T>
	FArcActivityTaggedData(FGameplayTag InTag, T InValue)
		: Tag(InTag)
		, Value(TInPlaceType<T>{}, InValue)
	{
	}

	FArcActivityTaggedData(FGameplayTag InTag, FTaggedDataVariant InData)
		: Tag(InTag)
		, Value(InData)
	{
		
	}

	FString GetDebugString() const;

private:
	friend struct FArcActivityTaggedDataContainer;

	UPROPERTY()
		FGameplayTag Tag;

	FTaggedDataVariant Value;
};


USTRUCT(BlueprintType)
struct FArcActivityTaggedDataContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	FArcActivityTaggedDataContainer()
	{
	}

public:
	template<typename T>
	void SetTaggedData(FGameplayTag Tag, T Value)
	{
		if (auto* Data = TaggedData.FindByPredicate([Tag](const FArcActivityTaggedData& Data)
		{
			return Data.Tag == Tag;
		}))
		{
			Data->Value.template Emplace<T>(Value);
			MarkItemDirty(*Data);
			TagToDataMap[Tag].Emplace<T>(Value);
		}
		else
		{
			FArcActivityTaggedData&  EmplacedData = TaggedData.Emplace_GetRef(Tag, Value);
			MarkItemDirty(EmplacedData);
			TagToDataMap[Tag].Emplace<T>(Value);
		}
	}

	void ClearTaggedData(FGameplayTag Tag)
	{
		for (auto Itr = TagToDataMap.CreateIterator(); Itr; ++Itr)
		{
			if (Tag == Itr->Key)
			{
				Itr.RemoveCurrent();
				TagToDataMap.Remove(Tag);
				MarkArrayDirty();
				return;
			}
		}
	}

	template<typename T>
	TOptional<T> GetTaggedData(FGameplayTag Tag) const
	{
		if (auto* Data = TaggedData.FindByPredicate([Tag](const FArcActivityTaggedData& Data)
		{
			return Data.Tag == Tag;
		}))
		{
			if (Data->Value.template IsType<T>())
			{
				return Data->Value.template Get<T>();
			}
		}

		return {};
	}

	bool HasTaggedData(FGameplayTag Tag) const
	{
		return TagToDataMap.Contains(Tag);
	}
	
	void RebuildTagToCountMap();

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract


	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FArcActivityTaggedData, FArcActivityTaggedDataContainer>(TaggedData, DeltaParms, *this);
	}

	ARC_ACT_FASTARRAYSERIALIZER_TARRAY_ACCESSORS(TaggedData)

private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FArcActivityTaggedData> TaggedData;

	// Accelerated list of tag stacks for queries
	TMap<FGameplayTag, FTaggedDataVariant> TagToDataMap;
};

template<>
struct TStructOpsTypeTraits<FArcActivityTaggedDataContainer> : public TStructOpsTypeTraitsBase2<FArcActivityTaggedDataContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};


USTRUCT()
struct FArcActivityPlayerEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

		FArcActivityPlayerEntry()
		: FArcActivityPlayerEntry(nullptr)
	{
	}

	FArcActivityPlayerEntry(UArcActivityPlayerComponent* InPlayer)
		: Super()
		, Player(InPlayer)
	{
	}

	UPROPERTY()
		UArcActivityPlayerComponent* Player;

	TWeakObjectPtr<UArcActivityPlayerComponent> PreviousPlayer;



	void PreReplicatedRemove(const struct FArcActivityPlayerArray& InArraySerializer);
	void PostReplicatedAdd(const struct FArcActivityPlayerArray& InArraySerializer);
	void PostReplicatedChange(const struct FArcActivityPlayerArray& InArraySerializer);

	// Optional: debug string used with LogNetFastTArray logging
	FString GetDebugString();

};

USTRUCT()
struct FArcActivityPlayerArray : public FFastArraySerializer
{
	GENERATED_BODY()

		FArcActivityPlayerArray(UArcActivityInstance* InOwner)
		: Super()
		, Owner(InOwner)
	{

	}

	FArcActivityPlayerArray()
		: FArcActivityPlayerArray(nullptr)
	{

	}

	UPROPERTY()
		UArcActivityInstance* Owner;

	UPROPERTY()
		TArray<FArcActivityPlayerEntry>	Items;

	/** Step 4: Copy this, replace example with your names */
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FArcActivityPlayerEntry, FArcActivityPlayerArray>(Items, DeltaParms, *this);
	}

	void Add(UArcActivityPlayerComponent* Player);
	void Remove(UArcActivityPlayerComponent* Player);

	void Reset();

	ARC_ACT_FASTARRAYSERIALIZER_TARRAY_ACCESSORS(Items)
};

/** Step 5: Copy and paste this struct trait, replacing FExampleArray with your Step 2 struct. */
template<>
struct TStructOpsTypeTraits< FArcActivityPlayerArray > : public TStructOpsTypeTraitsBase2< FArcActivityPlayerArray >
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};



#undef ARC_ACT_FASTARRAYSERIALIZER_TARRAY_ACCESSORS