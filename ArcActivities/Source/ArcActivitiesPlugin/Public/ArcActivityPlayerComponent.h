// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info


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

	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	void JoinActivity(UArcActivityInstance* ActivityInstance);
	UFUNCTION(BlueprintCallable, Category = "Arc|Activity")
	void LeaveActivity(UArcActivityInstance* ActivityInstance);
private: 

	void OnPlayerJoinedActivity_Internal(UArcActivityInstance* ActivityInstance);
	void OnPlayerLeftActivity_Internal(UArcActivityInstance* ActivityInstance, bool ActivityEnding = false);

	TArray<TWeakObjectPtr<UArcActivityInstance>> Activities;
};
