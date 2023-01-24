// 2017-2018 Puny Human Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActivityTask_Base.generated.h"

class UArcActivityInstance;
class UActivity;

/**
 * 
 */
UCLASS(Abstract)
class ACTIVITIESPLUGIN_API UActivityTask_Base : public UObject
{
	GENERATED_BODY()
public:
	UActivityTask_Base(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure, Category="Arc|Activity")
	UArcActivityInstance* GetActivityInstance() const;

	UFUNCTION(BlueprintPure, Category = "Arc|Activity")
	UActivity* GetActivityAsset() const;

	UFUNCTION(BlueprintNativeEvent)
	void EndPlay(bool Canceled);
	virtual void EndPlay_Implementation(bool Canceled) { };

	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay();
	virtual void BeginPlay_Implementation() { };

private:
	TObjectPtr<UActivity> ActivityAsset;
};
