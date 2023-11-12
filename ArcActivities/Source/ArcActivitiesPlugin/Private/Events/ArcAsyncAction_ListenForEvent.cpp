// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


#include "Events/ArcAsyncAction_ListenForEvent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/AssertionMacros.h"
#include "UObject/Class.h"
#include "UObject/Field.h"
#include "UObject/Script.h"
#include "UObject/ScriptMacros.h"
#include "UObject/Stack.h"
#include "UObject/UnrealType.h"
#include "ArcActivityWorldSubsystem.h"

UArcAsyncAction_ListenForEvent* UArcAsyncAction_ListenForEvent::ListenForActivityMessage(UObject *WorldContextObject, FGameplayTag Channel, UScriptStruct *PayloadType, EArcActivityMessageMatch MatchType)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}

	UArcAsyncAction_ListenForEvent* Action = NewObject<UArcAsyncAction_ListenForEvent>();
	Action->WorldPtr = World;
	Action->ChannelToRegister = Channel;
	Action->MessageStructType = PayloadType;
	Action->MessageMatchType = MatchType;
	Action->RegisterWithGameInstance(World);

	return Action;
}

void UArcAsyncAction_ListenForEvent::Activate()
{
    if (UWorld* World = WorldPtr.Get())
	{
		if (UArcActivityWorldSubsystem::HasInstance(World))
		{
			UArcActivityWorldSubsystem& Router = UArcActivityWorldSubsystem::Get(World);

			TWeakObjectPtr<UArcAsyncAction_ListenForEvent> WeakThis(this);
			ListenerHandle = Router.RegisterListenerInternal(ChannelToRegister,
				[WeakThis](FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload)
				{
					if (UArcAsyncAction_ListenForEvent* StrongThis = WeakThis.Get())
					{
						StrongThis->HandleMessageReceived(Channel, StructType, Payload);
					}
				},
				MessageStructType.Get(),
				MessageMatchType);

			return;
		}
	}

	SetReadyToDestroy();
}

void UArcAsyncAction_ListenForEvent::SetReadyToDestroy()
{
    ListenerHandle.Unregister();

	Super::SetReadyToDestroy();
}

void UArcAsyncAction_ListenForEvent::HandleMessageReceived(FGameplayTag Channel, const UScriptStruct *StructType, const void *Payload)
{
    	if (!MessageStructType.Get() || (MessageStructType.Get() == StructType))
	{
		ReceivedMessagePayloadPtr = Payload;

		OnMessageReceived.Broadcast(this, Channel);

		ReceivedMessagePayloadPtr = nullptr;
	}

	if (!OnMessageReceived.IsBound())
	{
		// If the BP object that created the async node is destroyed, OnMessageReceived will be unbound after calling the broadcast.
		// In this case we can safely mark this receiver as ready for destruction.		
		SetReadyToDestroy();
	}
}

bool UArcAsyncAction_ListenForEvent::GetPayload(int32 &OutPayload)
{
    checkNoEntry();
	return false;
}

DEFINE_FUNCTION(UArcAsyncAction_ListenForEvent::execGetPayload)
{
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	void* MessagePtr = Stack.MostRecentPropertyAddress;
	FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	P_FINISH;

	bool bSuccess = false;

	// Make sure the type we are trying to get through the blueprint node matches the type of the message payload received.
	if ((StructProp != nullptr) && (StructProp->Struct != nullptr) && (MessagePtr != nullptr) && (StructProp->Struct == P_THIS->MessageStructType.Get()) && (P_THIS->ReceivedMessagePayloadPtr != nullptr))
	{
		StructProp->Struct->CopyScriptStruct(MessagePtr, P_THIS->ReceivedMessagePayloadPtr);
		bSuccess = true;
	}

	*(bool*)RESULT_PARAM = bSuccess;
}
