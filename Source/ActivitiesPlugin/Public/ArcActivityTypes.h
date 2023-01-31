// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.generated.h"

class UArcActivityInstance;
class UActivityStage;
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
struct FArcActivityEndedEventPayload
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category="Activity")
	UArcActivityInstance* ActivityInstance;
	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	EArcActivitySuccessState ActivityState;
};

USTRUCT(BlueprintType)
struct FArcActivityActivityStageChangedEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityInstance* ActivityInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UActivityStage* PreviousStage;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UActivityStage* CurrentStage;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	EArcActivityObjectiveTrackerState PreviousStageState;
};

USTRUCT(BlueprintType)
struct FArcActivityPlayerJoinedActivityEventPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityInstance* ActivityInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Activity")
	UArcActivityPlayerComponent* Player;
};
