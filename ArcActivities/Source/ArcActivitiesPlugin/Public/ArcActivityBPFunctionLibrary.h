// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	
	
};
