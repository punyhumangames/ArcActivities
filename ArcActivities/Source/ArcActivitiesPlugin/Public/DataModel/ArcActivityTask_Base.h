// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See LICENSE-MIT and LICENSE-APACHE for more info

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArcActivityTask_Base.generated.h"

class UArcActivityInstance;
class UArcActivity;
class UArcActivityWorldSubsystem;

/**
 * 
 */
UCLASS(Abstract)
class ARCACTIVITIESPLUGIN_API UArcActivityTask_Base : public UObject
{
	GENERATED_BODY()
public:
	UArcActivityTask_Base(const FObjectInitializer& ObjectInitializer);


	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual bool IsNameStableForNetworking() const override { return false; }
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, struct FReplicationFlags* RepFlags) {
		return false;
	}

	virtual class UWorld* GetWorld() const;

	UFUNCTION(BlueprintPure, Category="Arc|Activity")
	UArcActivityInstance* GetActivityInstance() const;

	UFUNCTION(BlueprintPure, Category = "Arc|Activity")
	UArcActivity* GetActivityAsset() const;

	UArcActivityWorldSubsystem* GetSubsystem() const;

	UFUNCTION(BlueprintNativeEvent)
	void EndPlay(bool Canceled);
	virtual void EndPlay_Implementation(bool Canceled) { };

	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay();
	virtual void BeginPlay_Implementation() { };

private:
	TObjectPtr<UArcActivity> ActivityAsset;
};
