// Copyright 2023 Puny Human.  All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArcActivityPlayerComponent.generated.h"

class UArcActivityInstance;

UCLASS( ClassGroup=(ArcActivities), meta=(BlueprintSpawnableComponent) )
class ARCACTIVITIESPLUGIN_API UArcActivityPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	friend class UArcActivityInstance;

	UArcActivityPlayerComponent();

	UFUNCTION(BlueprintCallable)
	void JoinActivity(UArcActivityInstance* ActivityInstance);
	UFUNCTION(BlueprintCallable)
	void LeaveActivity(UArcActivityInstance* ActivityInstance);
private: 

	void OnPlayerJoinedActivity_Internal(UArcActivityInstance* ActivityInstance);
	void OnPlayerLeftActivity_Internal(UArcActivityInstance* ActivityInstance, bool ActivityEnding = false);

	TArray<TWeakObjectPtr<UArcActivityInstance>> Activities;
};
