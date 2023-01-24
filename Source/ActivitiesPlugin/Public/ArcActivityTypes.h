// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ArcActivityTypes.generated.h"

UENUM(BlueprintType)
enum class EArcActivityObjectState : uint8
{
	Tracking,
	CompletedSuccess,
	CompletedFail,
};

UENUM(BlueprintType)
enum class EArcActivityTransitionMode : uint8
{
	Success,
	Failure,
};