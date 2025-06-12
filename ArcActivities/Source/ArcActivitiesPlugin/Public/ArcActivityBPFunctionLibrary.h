// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArcActivityTypes.h"
#include "ArcActivityBPFunctionLibrary.generated.h"


class UArcActivityInstance;
/**
 * 
 */
UCLASS()
class ARCACTIVITIESPLUGIN_API UArcActivityBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// Returns the tagged data of an activity instance as an integer
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static int32 GetTaggedDataAsInt(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	// Returns the tagged data of an activity instance as a float
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static float GetTaggedDataAsFloat(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	// Returns the tagged data of an activity instance as a double
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static double GetTaggedDataAsDouble( UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	// Returns the tagged data of an activity instance as a Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static FGameplayTag GetTaggedDataAsGameplayTag(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	// Returns the tagged data of an activity instance as a Vector
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static FVector GetTaggedDataAsFVector(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	// Returns the tagged data of an activity instance as an Actor
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static AActor* GetTaggedDataAsActor(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found);

	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetIntTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetFloatTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, float Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetDoubleTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, double Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetGameplayTagTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, FGameplayTag Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetFVectorTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, FVector Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetActorTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, AActor* Value);

	
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsInt(UArcActivityInstance* Activity, FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsFloat(UArcActivityInstance* Activity, FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsDouble(UArcActivityInstance* Activity, FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsGameplayTag(UArcActivityInstance* Activity, FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsFVector(UArcActivityInstance* Activity, FGameplayTag Tag);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasTaggedDataAsActor(UArcActivityInstance* Activity, FGameplayTag Tag);

	// Returns the tagged data of an activity event as an integer
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static int32 GetPreviousTaggedDataAsInt_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	// Returns the tagged data of an activity event as a float
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static float GetPreviousTaggedDataAsFloat_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	// Returns the tagged data of an activity event as a double
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static double GetPreviousTaggedDataAsDouble_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	// Returns the tagged data of an activity event as a Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static FGameplayTag GetPreviousTaggedDataAsGameplayTag_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	// Returns the tagged data of an activity event as a Vector
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static FVector GetPreviousTaggedDataAsFVector_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	// Returns the tagged data of an activity event as an Actor
	UFUNCTION(BlueprintCallable, Category = "Activity", meta=(ExpandBoolAsExecs="Found"))
	static AActor* GetPreviousTaggedDataAsActor_Event(const FArcActivityTagStackChanged& Event, bool& Found);

	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetIntPreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetFloatPreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, float Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetDoublePreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, double Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetGameplayTagPreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, FGameplayTag Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetFVectorPreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, FVector Value);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static void SetActorPreviousTaggedData_Event(UPARAM(ref) FArcActivityTagStackChanged& Event, AActor* Value);

	
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsInt_Event(const FArcActivityTagStackChanged& Event);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsFloat_Event(const FArcActivityTagStackChanged& Event);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsDouble_Event(const FArcActivityTagStackChanged& Event);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsGameplayTag_Event(const FArcActivityTagStackChanged& Event);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsFVector_Event(const FArcActivityTagStackChanged& Event);
	UFUNCTION(BlueprintCallable, Category = "Activity")
	static bool HasPreviousTaggedDataAsActor_Event(const FArcActivityTagStackChanged& Event);
	
};
