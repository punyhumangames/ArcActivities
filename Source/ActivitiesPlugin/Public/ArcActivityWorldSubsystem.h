// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "ArcActivityWorldSubsystem.generated.h"

class UActivity;
class UArcActivityInstance;
class UArcActivityPlayerComponent;
class APlayerState;

/**
 * 
 */
UCLASS()
class ACTIVITIESPLUGIN_API UArcActivityWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UArcActivityPlayerComponent* RegisterPlayerForActivities(APlayerState* PlayerState) const;

	UFUNCTION(BlueprintCallable)
	UArcActivityInstance* StartActivity(UActivity* Activity, FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable)
		bool IsActivityActive(UArcActivityInstance* Activity) const;

	UFUNCTION(BlueprintCallable)
		void CancelActivity(UArcActivityInstance* Activity);

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:

	void OnActivityEndedEvent(UArcActivityInstance* Instance, bool bWasCancelled);

	TArray<UArcActivityInstance*> ActivityInstances;
};
