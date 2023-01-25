// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.generated.h"

UENUM(BlueprintType)
enum class EArcActivityObjectiveTrackerState : uint8
{
	Tracking,
	CompletedSuccess,
	CompletedFail,
};

UENUM(BlueprintType)
enum class EArcActivitySuccessState : uint8
{
	Success,
	Failure,
	Cancelled,
	InProgress,
};


UENUM(BlueprintType)
enum class EArcActivityCompletionMode : uint8
{	
	//Will mark this as success if any child has succeeded. 
	AnySuccess,
	//Will mark as success if all are successful, fail if any failed
	AllSuccess,
};