// 2017-2019 Puny Human

#pragma once

#include "Delegates/Delegate.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "ArcActivityWorldSubsystem.h"

#include "ArcAsyncAction_ListenForEvent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArcActivityEventDelegate, UArcAsyncAction_ListenForEvent*, Listener, FGameplayTag, Channel);


UCLASS(BlueprintType, meta=(HasDedicatedAsyncNode))
class ACTIVITIESPLUGIN_API UArcAsyncAction_ListenForEvent : public UCancellableAsyncAction
{
    GENERATED_BODY()
public:
	/**
	 * Asynchronously waits for a gameplay message to be broadcast on the specified channel.
	 *
	 * @param Channel			The message channel to listen for
	 * @param PayloadType		The kind of message structure to use (this must match the same type that the sender is broadcasting)
	 * @param MatchType			The rule used for matching the channel with broadcasted messages
	 */
	UFUNCTION(BlueprintCallable, Category = Messaging, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static UArcAsyncAction_ListenForEvent* ListenForActivityMessage(UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType, EArcActivityMessageMatch MatchType = EArcActivityMessageMatch::ExactMatch);

	/**
	 * Attempt to copy the payload received from the broadcasted gameplay message into the specified wildcard.
	 * The wildcard's type must match the type from the received message.
	 *
	 * @param OutPayload	The wildcard reference the payload should be copied into
	 * @return				If the copy was a success
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Messaging", meta = (CustomStructureParam = "OutPayload"))
	bool GetPayload(UPARAM(ref) int32& OutPayload);

	DECLARE_FUNCTION(execGetPayload);

	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

public:
	/** Called when a message is broadcast on the specified channel. Use GetPayload() to request the message payload. */
	UPROPERTY(BlueprintAssignable)
	FArcActivityEventDelegate OnMessageReceived;

private:
	void HandleMessageReceived(FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload);

private:
	const void* ReceivedMessagePayloadPtr = nullptr;

	TWeakObjectPtr<UWorld> WorldPtr;
	FGameplayTag ChannelToRegister;
	TWeakObjectPtr<UScriptStruct> MessageStructType = nullptr;
	EArcActivityMessageMatch MessageMatchType = EArcActivityMessageMatch::ExactMatch;

	FArcActivityMessageListenerHandle ListenerHandle;

};