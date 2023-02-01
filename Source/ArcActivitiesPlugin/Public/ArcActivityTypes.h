// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "ArcActivityTypes.generated.h"


ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityStateChangedEventTag);
ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityStageChangedEventTag);
ARCACTIVITIESPLUGIN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FArcActivityPlayerChangedEventTag);


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
		EArcActivitySuccessState ActivityState;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	EArcActivitySuccessState PreviousActivityState;

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
		EArcActivitySuccessState PreviousStageState;

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
		EArcActivityPlayerEventType EventType;

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
