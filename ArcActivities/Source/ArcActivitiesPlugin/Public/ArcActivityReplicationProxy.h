// Arc Activities Plugin.  Copyright Puny Human and Garrett Fleenor.  Licensed under MIT or Apache 2.0 license.  See
// LICENSE-MIT and LICENSE-APACHE for more info

#pragma once

#include "CoreMinimal.h"
#include "Engine/NetSerialization.h"
#include "GameFramework/Actor.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ArcActivityReplicationProxy.generated.h"

class UArcActivityInstance;
class UArcActivity;

USTRUCT()
struct FArcActivityReplicationEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

    FArcActivityReplicationEntry() : FArcActivityReplicationEntry(nullptr)
    {
    }

    FArcActivityReplicationEntry(UArcActivityInstance *InActivity) : Super(), Activity(InActivity)
    {
    }

    UPROPERTY()
    UArcActivityInstance *Activity;

    TWeakObjectPtr<UArcActivityInstance> PreviousActivity;

    void PreReplicatedRemove(const struct FArcActivityReplicationArray &InArraySerializer);
    void PostReplicatedAdd(const struct FArcActivityReplicationArray &InArraySerializer);
    void PostReplicatedChange(const struct FArcActivityReplicationArray &InArraySerializer);

    // Optional: debug string used with LogNetFastTArray logging
    FString GetDebugString();
};

class AArcActivityReplicationProxy;

USTRUCT()
struct FArcActivityReplicationArray : public FFastArraySerializer
{
    GENERATED_BODY()

    FArcActivityReplicationArray(AArcActivityReplicationProxy *InRepProxy) : RepProxy(InRepProxy)
    {
    }
    FArcActivityReplicationArray() : FArcActivityReplicationArray(nullptr)
    {
    }

    UPROPERTY()
    TArray<FArcActivityReplicationEntry> Items;
    UPROPERTY()
    AArcActivityReplicationProxy *RepProxy;

    /** Step 4: Copy this, replace example with your names */
    bool NetDeltaSerialize(FNetDeltaSerializeInfo &DeltaParms)
    {
        return FFastArraySerializer::FastArrayDeltaSerialize<FArcActivityReplicationEntry,
                                                             FArcActivityReplicationArray>(Items, DeltaParms, *this);
    }

    auto begin()
    {
        return Items.begin();
    }
    auto begin() const
    {
        return Items.begin();
    }
    auto end()
    {
        return Items.end();
    }
    auto end() const
    {
        return Items.end();
    }
    auto &operator[](int32 index)
    {
        return Items[index];
    }
    const auto &operator[](int32 index) const
    {
        return Items[index];
    }
    int32 Num() const
    {
        return Items.Num();
    }
};

/** Step 5: Copy and paste this struct trait, replacing FExampleArray with your Step 2 struct. */
template <>
struct TStructOpsTypeTraits<FArcActivityReplicationArray>
    : public TStructOpsTypeTraitsBase2<FArcActivityReplicationArray>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};

UCLASS()
class ARCACTIVITIESPLUGIN_API AArcActivityReplicationProxy : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AArcActivityReplicationProxy();

    virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch,
                                     FReplicationFlags *RepFlags) override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> &OutLifetimeProps) const override;

    void AddReplicatedActivityInstance(UArcActivityInstance *Instance);
    void RemoveReplicatedActivityInstance(UArcActivityInstance *Instance);

  protected:
    UPROPERTY(Replicated)
    FArcActivityReplicationArray ReplicatedActivities;
};
