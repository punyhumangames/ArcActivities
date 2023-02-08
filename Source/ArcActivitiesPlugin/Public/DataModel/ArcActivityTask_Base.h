// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArcActivityTask_Base.generated.h"

class UArcActivityInstance;
class UArcActivity;

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
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) {
		return false;
	}

	virtual class UWorld* GetWorld() const;

	UFUNCTION(BlueprintPure, Category="Arc|Activity")
	UArcActivityInstance* GetActivityInstance() const;

	UFUNCTION(BlueprintPure, Category = "Arc|Activity")
	UArcActivity* GetActivityAsset() const;

	UFUNCTION(BlueprintNativeEvent)
	void EndPlay(bool Canceled);
	virtual void EndPlay_Implementation(bool Canceled) { };

	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay();
	virtual void BeginPlay_Implementation() { };

private:
	TObjectPtr<UArcActivity> ActivityAsset;
};
