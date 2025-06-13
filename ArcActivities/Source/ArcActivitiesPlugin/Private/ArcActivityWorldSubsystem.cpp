// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info



#include "ArcActivityWorldSubsystem.h"
#include "ArcActivitiesPlugin.h"
#include "Engine/Engine.h"

#include "DataModel/ArcActivity.h"
#include "GameFramework/PlayerState.h"
#include "ArcActivityPlayerComponent.h"
#include "ArcActivityInstance.h"
#include "Engine/World.h"

#include "ArcActivityReplicationProxy.h"

namespace FActivityWorld
{
	struct FActivityScopeLock
	{
		FActivityScopeLock(UArcActivityWorldSubsystem* InSubsys)
			: Subsys(InSubsys)
		{
			Subsys->ScopeLocks++;
		}

		~FActivityScopeLock()
		{
			Subsys->ScopeLocks--;
			if (Subsys->ScopeLocks == 0)
			{
				//Run the actions that were saved
				for (const auto& Action : Subsys->LockQueuedActions)
				{
					Action(Subsys);
				}
				Subsys->LockQueuedActions.Reset();
			}
		}

		UArcActivityWorldSubsystem* Subsys;
	};
}

void FArcActivityMessageListenerHandle::Unregister()
{
    if (UArcActivityWorldSubsystem* StrongSubsystem = Subsystem.Get())
	{
		StrongSubsystem->UnregisterListener(*this);
		Subsystem.Reset();
		Channel = FGameplayTag();
		ID = 0;
	}
}



UArcActivityWorldSubsystem::UArcActivityWorldSubsystem()
	: Super()
{
	ScopeLocks = 0;
}

UArcActivityWorldSubsystem &UArcActivityWorldSubsystem::Get(const UObject *WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	check(World);
	UArcActivityWorldSubsystem* Router = World->GetSubsystem<UArcActivityWorldSubsystem>();
	check(Router);
	return *Router;
}

bool UArcActivityWorldSubsystem::HasInstance(const UObject *WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	UArcActivityWorldSubsystem* Router = World != nullptr ? World->GetSubsystem<UArcActivityWorldSubsystem>() : nullptr;
	return Router != nullptr;
}

void UArcActivityWorldSubsystem::OnWorldBeginPlay(UWorld &InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

	//If we are a server, we need to create a replication proxy for the activities so they properly replicate
	if (InWorld.GetNetMode() == ENetMode::NM_DedicatedServer || InWorld.GetNetMode() == ENetMode::NM_ListenServer)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ReplicationProxy = InWorld.SpawnActor<AArcActivityReplicationProxy>(SpawnParams);
	}
}

void UArcActivityWorldSubsystem::Deinitialize()
{
    ListenerMap.Reset();
	{
		FActivityWorld::FActivityScopeLock Lock(this);

		for (UArcActivityInstance* Instance : ActivityInstances)
		{
			if (IsValid(Instance))
			{
				Instance->OnActivityEnded.RemoveAll(this); //We don't want to hear the dying screams of these activities as we shut them down.
				Instance->EndActivity(true);
			}

		}
		ActivityInstances.Reset();
	}

	if (ReplicationProxy)
	{
		ReplicationProxy->Destroy();
	}

    Super::Deinitialize();
}

UArcActivityPlayerComponent* UArcActivityWorldSubsystem::RegisterPlayerForActivities(APlayerState* PlayerState) 
{
    if (!IsValid(PlayerState))
    {
        return nullptr;
    }
	if (UArcActivityPlayerComponent* ActivityComp = PlayerState->FindComponentByClass<UArcActivityPlayerComponent>())
	{
		return ActivityComp;
	}

    if (UArcActivityPlayerComponent* ActivityComp = NewObject<UArcActivityPlayerComponent>(PlayerState))
    {
		ActivityComp->RegisterComponent();
        return ActivityComp;
    }

    return nullptr;
   
}

UArcActivityInstance* UArcActivityWorldSubsystem::StartActivity(UArcActivity* Activity, const FArcActivityParameters& Params)
{
	UObject* InstanceOuter = this;
	if(IsValid(ReplicationProxy))
	{
		InstanceOuter = ReplicationProxy;
	}

   
    if (UArcActivityInstance* Instance = NewObject<UArcActivityInstance>(InstanceOuter))
    {
        Instance->World = GetWorld();
		Instance->ImportTaggedData(Params.TaggedData);
    	
		if (Instance->InitActivityGraph(Activity, Params.Tags))
		{
			Instance->OnActivityEnded.AddUObject(this, &ThisClass::OnActivityEndedEvent);
			ActivityInstances.AddUnique(Instance);

			if (IsValid(ReplicationProxy))
			{
				ReplicationProxy->AddReplicatedActivityInstance(Instance);
			}

			//TODO: Report the creation of this activity instance
			return Instance;
		}       
    }
    return nullptr;
}

bool UArcActivityWorldSubsystem::IsActivityActive(UArcActivityInstance* Activity) const
{
    if (IsValid(Activity))
    {
        return Activity->IsActive();
    }
    return false;
}

void UArcActivityWorldSubsystem::CancelActivity(UArcActivityInstance* Activity)
{
    if (IsValid(Activity) && Activity->IsActive())
    {
        Activity->EndActivity(true);
    }
}

void UArcActivityWorldSubsystem::ForEachActivity(TFunctionRef<ForEachActivityPredicate> Pred) const
{
	for (const UArcActivityInstance* Instance : ActivityInstances)
	{
		if (IsValid(Instance))
		{
			Pred(Instance);
		}
	}
}

void UArcActivityWorldSubsystem::ForEachActivity_Mutable(TFunctionRef<ForEachActivityPredicate_Mutable> Pred) const
{
	for (UArcActivityInstance* Instance : ActivityInstances)
	{
		if (IsValid(Instance))
		{
			Pred(Instance);
		}
	}
}


bool UArcActivityWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UArcActivityWorldSubsystem::OnActivityEndedEvent(UArcActivityInstance* Instance, bool bWasCancelled)
{
	auto RemoveAction = [Instance, bWasCancelled](UArcActivityWorldSubsystem* Subsys)
	{
		Instance->OnActivityEnded.RemoveAll(Subsys);
		Subsys->ActivityInstances.Remove(Instance);

		if (IsValid(Subsys->ReplicationProxy))
		{
			Subsys->ReplicationProxy->RemoveReplicatedActivityInstance(Instance);
		}
	};

	if (ScopeLocks > 0)
	{
		LockQueuedActions.Add(RemoveAction);
	}
	else
	{
		RemoveAction(this);
	}   
}

void UArcActivityWorldSubsystem::NotifyAddedActivityFromReplication(UArcActivityInstance* Instance)
{
	if (IsValid(Instance))
	{
		Instance->World = GetWorld();
		ActivityInstances.Add(Instance);
		BroadcastMessage(FArcActivityStateChangedEventTag, FArcActivityActivityStateChanged(Instance, EArcActivitySuccessState::InProgress, EArcActivitySuccessState::None));
		BroadcastMessage(FArcActivityStageChangedEventTag, FArcActivityStageChangedEventPayload(Instance, Instance->CurrentStage, nullptr, EArcActivitySuccessState::None));
	}
}

void UArcActivityWorldSubsystem::NotifyRemovedActivityFromReplication(UArcActivityInstance* Instance)
{
	if (IsValid(Instance))
	{
		ActivityInstances.RemoveSwap(Instance);
		BroadcastMessage(FArcActivityStateChangedEventTag, FArcActivityActivityStateChanged(Instance, Instance->ActivityState, EArcActivitySuccessState::InProgress));

	}

}

namespace Arc
{
	namespace ActivityWorldSubsystem
	{
		static int32 ShouldLogMessages = 0;
		static FAutoConsoleVariableRef CVarShouldLogMessages(TEXT("ArcActivityWorldSubsystem.LogMessages"),
			ShouldLogMessages,
			TEXT("Should messages broadcast through the Activity Plugin be logged?"));
	}
}

void UArcActivityWorldSubsystem::UnregisterListener(FArcActivityMessageListenerHandle Handle)
{
    if (Handle.IsValid())
	{
		check(Handle.Subsystem == this);

		UnregisterListenerInternal(Handle.Channel, Handle.ID);
	}
	else
	{
		UE_LOG(LogActivitiesPlugin, Warning, TEXT("Trying to unregister an invalid Handle."));
	}
}

void UArcActivityWorldSubsystem::K2_BroadcastMessage(FGameplayTag Channel, const int32 &Message)
{
    checkNoEntry();
}
DEFINE_FUNCTION(UArcActivityWorldSubsystem::execK2_BroadcastMessage)
{
	P_GET_STRUCT(FGameplayTag, Channel);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	void* MessagePtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;

	if (ensure((StructProp != nullptr) && (StructProp->Struct != nullptr) && (MessagePtr != nullptr)))
	{
		P_THIS->BroadcastMessageInternal(Channel, StructProp->Struct, MessagePtr);
	}
}

void UArcActivityWorldSubsystem::BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct *StructType, const void *MessageBytes)
{
    	// Log the message if enabled
	if (Arc::ActivityWorldSubsystem::ShouldLogMessages != 0)
	{
		FString* pContextString = nullptr;
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			pContextString = &GPlayInEditorContextString;
		}
#endif

		FString HumanReadableMessage;
		StructType->ExportText(/*out*/ HumanReadableMessage, MessageBytes, /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
		UE_LOG(LogActivitiesPlugin, Log, TEXT("BroadcastMessage(%s, %s, %s)"), pContextString ? **pContextString : *GetPathNameSafe(this), *Channel.ToString(), *HumanReadableMessage);
	}

	// Broadcast the message
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Channel; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FArcChannelListenerList* pList = ListenerMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			TArray<FArcActivityMessageListenerData> ListenerArray(pList->Listeners);

			for (const FArcActivityMessageListenerData& Listener : ListenerArray)
			{
				if (bOnInitialTag || (Listener.MatchType == EArcActivityMessageMatch::PartialMatch))
				{
					if (Listener.bHadValidType && !Listener.ListenerStructType.IsValid())
					{
						UE_LOG(LogActivitiesPlugin, Warning, TEXT("Listener struct type has gone invalid on Channel %s. Removing listener from list"), *Channel.ToString());
						UnregisterListenerInternal(Channel, Listener.HandleID);
						continue;
					}

					// The receiving type must be either a parent of the sending type or completely ambiguous (for internal use)
					if (!Listener.bHadValidType || StructType->IsChildOf(Listener.ListenerStructType.Get()))
					{
						Listener.ReceivedCallback(Channel, StructType, MessageBytes);
					}
					else
					{
						UE_LOG(LogActivitiesPlugin, Error, TEXT("Struct type mismatch on channel %s (broadcast type %s, listener at %s was expecting type %s)"),
							*Channel.ToString(),
							*StructType->GetPathName(),
							*Tag.ToString(),
							*Listener.ListenerStructType->GetPathName());
					}
				}
			}
		}
		bOnInitialTag = false;
	}
}

FArcActivityMessageListenerHandle UArcActivityWorldSubsystem::RegisterListenerInternal(FGameplayTag Channel, TFunction<void(FGameplayTag, const UScriptStruct *, const void *)> &&Callback, const UScriptStruct *StructType, EArcActivityMessageMatch MatchType)
{
    FArcChannelListenerList& List = ListenerMap.FindOrAdd(Channel);

	FArcActivityMessageListenerData& Entry = List.Listeners.AddDefaulted_GetRef();
	Entry.ReceivedCallback = MoveTemp(Callback);
	Entry.ListenerStructType = StructType;
	Entry.bHadValidType = StructType != nullptr;
	Entry.HandleID = ++List.HandleID;
	Entry.MatchType = MatchType;

	return FArcActivityMessageListenerHandle(this, Channel, Entry.HandleID);
}

void UArcActivityWorldSubsystem::UnregisterListenerInternal(FGameplayTag Channel, int32 HandleID)
{
    if (FArcChannelListenerList* pList = ListenerMap.Find(Channel))
	{
		int32 MatchIndex = pList->Listeners.IndexOfByPredicate([ID = HandleID](const FArcActivityMessageListenerData& Other) { return Other.HandleID == ID; });
		if (MatchIndex != INDEX_NONE)
		{
			pList->Listeners.RemoveAtSwap(MatchIndex);
		}

		if (pList->Listeners.Num() == 0)
		{
			ListenerMap.Remove(Channel);
		}
	}
}

