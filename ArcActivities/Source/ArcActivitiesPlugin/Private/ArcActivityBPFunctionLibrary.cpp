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
			Found = true;
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
			Found = true;
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
			Found = true;
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
			Found = true;
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
			Found = true;
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
				Found = true;
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

int32 UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsInt_Event(const FArcActivityTagStackChanged& Event,
	bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<int32>())
	{
		Found = true;
		return *Value;
	}
	Found = false;
	return {};
}

float UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsFloat_Event(const FArcActivityTagStackChanged& Event,
	bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<float>())
	{
		Found = true;
		return *Value;
	}
	Found = false;
	return {};
}

double UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsDouble_Event(const FArcActivityTagStackChanged& Event,
	bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<double>())
	{
		Found = true;
		return *Value;
	}
	Found = false;
	return {};
}

FGameplayTag UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsGameplayTag_Event(
	const FArcActivityTagStackChanged& Event, bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<FGameplayTag>())
	{
		Found = true;
		return *Value;
	}
	Found = false;
	return {};
}

FVector UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsFVector_Event(const FArcActivityTagStackChanged& Event,
	bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<FVector>())
	{
		Found = true;
		return *Value;
	}
	Found = false;
	return {};
}

AActor* UArcActivityBPFunctionLibrary::GetPreviousTaggedDataAsActor_Event(const FArcActivityTagStackChanged& Event,
	bool& Found)
{
	if (auto Value = Event.PreviousValue.TryGet<TWeakObjectPtr<AActor>>();
		Value != nullptr && Value->IsValid())
	{		
		Found = true;
		return Value->Get();			
	}
	Found = false;
	return {};
}

void UArcActivityBPFunctionLibrary::SetIntPreviousTaggedData_Event(FArcActivityTagStackChanged& Event, int32 Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<int32>{}, Value);
}

void UArcActivityBPFunctionLibrary::SetFloatPreviousTaggedData_Event(FArcActivityTagStackChanged& Event, float Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<float>{}, Value);
}

void UArcActivityBPFunctionLibrary::SetDoublePreviousTaggedData_Event(FArcActivityTagStackChanged& Event, double Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<double>{}, Value);
}

void UArcActivityBPFunctionLibrary::SetGameplayTagPreviousTaggedData_Event(FArcActivityTagStackChanged& Event,
	FGameplayTag Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<FGameplayTag>{}, Value);
}

void UArcActivityBPFunctionLibrary::SetFVectorPreviousTaggedData_Event(FArcActivityTagStackChanged& Event,
	FVector Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<FVector>{}, Value);
}

void UArcActivityBPFunctionLibrary::SetActorPreviousTaggedData_Event(FArcActivityTagStackChanged& Event, AActor* Value)
{
	Event.PreviousValue = FTaggedDataVariant(TInPlaceType<TWeakObjectPtr<AActor>>{}, Value);
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsInt_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<int>();
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsFloat_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<float>();
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsDouble_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<double>();
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsGameplayTag_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<FGameplayTag>();
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsFVector_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<FVector>();
}

bool UArcActivityBPFunctionLibrary::HasPreviousTaggedDataAsActor_Event(const FArcActivityTagStackChanged& Event)
{
	return Event.PreviousValue.IsType<TWeakObjectPtr<AActor>>();
}

