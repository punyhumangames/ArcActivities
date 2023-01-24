// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcActivityWorldSubsystem.h"
#include "DataModel/Activity.h"
#include "GameFramework/PlayerState.h"
#include "ArcActivityPlayerComponent.h"
#include "ArcActivityInstance.h"

void UArcActivityWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
}

UArcActivityPlayerComponent* UArcActivityWorldSubsystem::RegisterPlayerForActivities(APlayerState* PlayerState) const
{
    if (!IsValid(PlayerState))
    {
        return nullptr;
    }

    if (UArcActivityPlayerComponent* ActivitiyComp = NewObject<UArcActivityPlayerComponent>(PlayerState))
    {
        ActivitiyComp->RegisterComponent();
        return ActivitiyComp;
    }

    return nullptr;
   
}

UArcActivityInstance* UArcActivityWorldSubsystem::StartActivity(UActivity* Activity, FGameplayTagContainer Tags)
{
    UArcActivityInstance* Instance = NewObject<UArcActivityInstance>(this); //TODO: for replication, make this outer some replicated 
    if (IsValid(Instance))
    {
        Instance->OnActivityEnded.AddUObject(this, &ThisClass::OnActivityEndedEvent);
        Instance->InitActivityGraph(Activity, Tags);
        ActivityInstances.AddUnique(Instance);

        //TODO: Report the creation of this activity instance
        return Instance;
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
        Activity->EndActivity();
    }
}

bool UArcActivityWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UArcActivityWorldSubsystem::OnActivityEndedEvent(UArcActivityInstance* Instance, bool bWasCancelled)
{
    Instance->OnActivityEnded.RemoveAll(this);
    ActivityInstances.Remove(Instance);
}
