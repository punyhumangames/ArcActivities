// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcActivityBPFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "ArcActivityInstance.h"

int32 UArcActivityBPFunctionLibrary::GetTaggedDataAsInt(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<int32>(Tag))
		{
			return *Value;
		}
	}
	Found = false;
	return {};
}

float UArcActivityBPFunctionLibrary::GetTaggedDataAsFloat(UArcActivityInstance* Activity, FGameplayTag Tag, bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<float>(Tag))
		{
			return *Value;
		}
	}
	Found = false;
	return {};
}

double UArcActivityBPFunctionLibrary::GetTaggedDataAsDouble(UArcActivityInstance* Activity, FGameplayTag Tag,
	bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<double>(Tag))
		{
			return *Value;
		}
	}
	Found = false;
	return {};
}

FGameplayTag UArcActivityBPFunctionLibrary::GetTaggedDataAsGameplayTag(UArcActivityInstance* Activity, FGameplayTag Tag,
	bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<FGameplayTag>(Tag))
		{
			return *Value;
		}
	}
	Found = false;
	return {};
}

FVector UArcActivityBPFunctionLibrary::GetTaggedDataAsFVector(UArcActivityInstance* Activity, FGameplayTag Tag,
	bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<FVector>(Tag))
		{
			return *Value;
		}
	}
	Found = false;
	return {};
}

AActor* UArcActivityBPFunctionLibrary::GetTaggedDataAsActor(UArcActivityInstance* Activity, FGameplayTag Tag,
	bool& Found)
{
	if (IsValid(Activity))
	{
		if (auto Value = Activity->GetTaggedData<TWeakObjectPtr<AActor>>(Tag))
		{
			if (Value->IsValid())
			{
				return Value->Get();
			}
		}
	}
	Found = false;
	return {};
}

void UArcActivityBPFunctionLibrary::SetIntTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, int32 Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, Value);
	}
}

void UArcActivityBPFunctionLibrary::SetFloatTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, float Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, Value);
	}
}

void UArcActivityBPFunctionLibrary::SetDoubleTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, double Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, Value);
	}
}

void UArcActivityBPFunctionLibrary::SetGameplayTagTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag,
	FGameplayTag Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, Value);
	}
}

void UArcActivityBPFunctionLibrary::SetFVectorTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag,
	FVector Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, Value);
	}
}

void UArcActivityBPFunctionLibrary::SetActorTaggedData(UArcActivityInstance* Activity, FGameplayTag Tag, AActor* Value)
{
	if (IsValid(Activity))
	{
		Activity->SetTaggedData(Tag, TWeakObjectPtr(Value));
	}
}


bool UArcActivityBPFunctionLibrary::HasTaggedDataAsInt(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<int32>(Tag);
}

bool UArcActivityBPFunctionLibrary::HasTaggedDataAsFloat(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<float>(Tag);
}

bool UArcActivityBPFunctionLibrary::HasTaggedDataAsDouble(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<double>(Tag);
}

bool UArcActivityBPFunctionLibrary::HasTaggedDataAsGameplayTag(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<FGameplayTag>(Tag);
}

bool UArcActivityBPFunctionLibrary::HasTaggedDataAsFVector(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<FVector>(Tag);
}

bool UArcActivityBPFunctionLibrary::HasTaggedDataAsActor(UArcActivityInstance* Activity, FGameplayTag Tag)
{
	return IsValid(Activity) && Activity->HasTaggedData<TWeakObjectPtr<AActor>>(Tag);
}

